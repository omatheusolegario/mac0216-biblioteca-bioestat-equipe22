# Arquivo makefile do projeto do grupo 22 
# Feito por Thiago Baisch

# Todos os .c de src/ viram .o em build/ (descobertos automaticamente)
# wildcard - faz um ls src/*.c aqui, essa variável vira todos os src/*.c
# patsubst faz um map(), em que pega todos os *.c que tem em src e vira build/*.o, assim,
# essa variável tem todos os arquivos build/*.o, com base no que está em src/*.c

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# -MMD -MP: o gcc escreve, ao lado de cada .o, um arquivo .d com a lista de headers que o .c inclui
DEPFLAGS = -MMD -MP

# Biblioteca matematica (sqrt, pow, log...)
LDLIBS = -lm

# Onde o 'make install' instala. O ?= so define se voce nao passou outro valor
#   make install PREFIX=$(HOME)/.local  
PREFIX ?= /usr/local

# Adaptação para conseguir testar no meu macOS... linux continua funcionando
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    SHARED_EXT   = dylib
    SHARED_FLAGS = -dynamiclib -install_name @rpath/libbioestat.dylib
    RPATH        = -Wl,-rpath,@executable_path/../lib
else
    SHARED_EXT   = so
    SHARED_FLAGS = -shared
    RPATH        = -Wl,-rpath,'$$ORIGIN/../lib'
endif

# Todos os .c de src/ viram .o em build/ 
LIB_SRCS = $(wildcard src/*.c)
LIB_OBJS = $(patsubst src/%.c,build/%.o,$(LIB_SRCS))
# Todos os .c de app/ viram .o em build/app/ (a aplicação tem vários arquivos: main.c, menus...)
APP_SRCS = $(wildcard app/*.c)
APP_OBJS = $(patsubst app/%.c,build/app/%.o,$(APP_SRCS))

STATIC_LIB = lib/libbioestat.a
SHARED_LIB = lib/libbioestat.$(SHARED_EXT)

# Todo arquivo tests/test_*.c vira um executavel de teste em bin/tests/
TEST_SRCS = $(wildcard tests/test_*.c)
TEST_OBJS = $(patsubst tests/%.c,build/tests/%.o,$(TEST_SRCS))
TEST_BINS = $(patsubst tests/%.c,bin/tests/%,$(TEST_SRCS))


all: bin/app_estatica bin/app_dinamica

.PHONY: all run test install uninstall clean doc

# qualquer build/x.o vem de src/x.c
# Para eu lembrar: $@ é o alvo, $< é a primeira dependência
# Exemplo: o comando vira:
# gcc -Wall -Wextra -std=c11 -Iinclude -c src/epidemiologia.c -o build/epidemiologia.o
# O -fPIC gera código que não assume endereços fixos
build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) $(DEPFLAGS) -fPIC -c $< -o $@

# qualquer build/app/x.o vem de app/x.c (a aplicação não leva -fPIC: não vai para a biblioteca)
build/app/%.o: app/%.c
	mkdir -p build/app
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Biblioteca estatica aqui: é um "pacote" com todos os .o da lib (sem o main.o)
# Alvo: a biblioteca. Pré requisitos: LIB_OBJS (que sao os .o em build/)
$(STATIC_LIB): $(LIB_OBJS)
	mkdir -p lib
	ar rcs $@ $^

# Biblioteca dinamica: os mesmos .o, ligados num unico arquivo carregado na execucao
$(SHARED_LIB): $(LIB_OBJS)
	mkdir -p lib
	$(CC) $(SHARED_FLAGS) -o $@ $^ $(LDLIBS)

# O app e ligado com a biblioteca,
bin/app_estatica: $(APP_OBJS) $(STATIC_LIB)
	mkdir -p bin
	$(CC) $(APP_OBJS) $(STATIC_LIB) $(LDLIBS) -o $@

# App dinamico: -L diz ONDE procurar, -l diz QUAL biblioteca, o rpath diz onde ela estara na EXECUCAO
# rpath - endereço dentro do executável que indica onde procurar a biblioteca (nesse caso, em /lib)
# Alternativa a exportar uma variável como LIBRARY_PATH
bin/app_dinamica: $(APP_OBJS) $(SHARED_LIB)
	mkdir -p bin
	$(CC) $(APP_OBJS) -Llib -lbioestat $(RPATH) $(LDLIBS) -o $@

# Parte dos testes:
# Compila cada tests/test_x.c em build/tests/test_x.o
build/tests/%.o: tests/%.c
	mkdir -p build/tests
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Sem isto o make apaga os .o dos testes depois de usar
.SECONDARY: $(TEST_OBJS)

# Liga cada teste com a biblioteca ESTATICA
bin/tests/%: build/tests/%.o $(STATIC_LIB)
	mkdir -p bin/tests
	$(CC) $< $(STATIC_LIB) $(LDLIBS) -o $@

# Roda todos os testes; falha (codigo de saida != 0) se algum falhar
test: $(TEST_BINS)
	@falhou=0; \
	for t in $(TEST_BINS); do \
		echo "=== $$t"; \
		./$$t || falhou=1; \
	done; \
	if [ $$falhou -eq 0 ]; then echo "RESULTADO: TODOS OS TESTES PASSARAM"; \
	else echo "RESULTADO: HA TESTES FALHANDO"; exit 1; fi


#  Executar a aplicacao com as duas bibliotecas
# --demo: roda a demonstracao sem pedir nada, pois o menu interativo ficaria esperando o teclado
run: all
	@echo "=== Aplicacao com biblioteca ESTATICA ==="
	./bin/app_estatica --demo
	@echo "=== Aplicacao com biblioteca DINAMICA ==="
	./bin/app_dinamica --demo

# Instalar / desinstalar 
# Instala as duas bibliotecas em $(PREFIX)/lib e os headers publicos em $(PREFIX)/include/bioestat
# install -d cria os diretorios, -m define as permissoes 
# Para usar depois:  gcc programa.c -I$(PREFIX)/include/bioestat -L$(PREFIX)/lib -lbioestat -lm
install: $(STATIC_LIB) $(SHARED_LIB)
	install -d $(PREFIX)/lib $(PREFIX)/include/bioestat
	install -m 644 $(STATIC_LIB) $(PREFIX)/lib
	install -m 755 $(SHARED_LIB) $(PREFIX)/lib
	install -m 644 include/*.h $(PREFIX)/include/bioestat
	@echo "Instalado em $(PREFIX)"

# Remove so o que o install colocou
uninstall:
	rm -f $(PREFIX)/lib/libbioestat.a $(PREFIX)/lib/libbioestat.$(SHARED_EXT)
	rm -rf $(PREFIX)/include/bioestat

clean:
	rm -rf build bin lib doc

doc:
	doxygen Doxyfile

# Carrega as dependencias geradas pelo gcc (o '-' ignora se ainda nao existirem)
-include $(LIB_OBJS:.o=.d) $(APP_OBJS:.o=.d) $(TEST_OBJS:.o=.d)