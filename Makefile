
# Makefile bem básico - versão para arquivos genéricos, podendo ser usado em todos os módulos
# OBS: não tem os .h aqui ainda, então mudanças nos .h ainda não vao ser vistas

# Todos os .c de src/ viram .o em build/ (descobertos automaticamente)
# wildcard - faz um ls src/*.c aqui, essa variável vira todos os src/*.c
# patsubst faz um map(), em que pega todos os *.c que tem em src e vira build/*.o, assim,
# essa variável tem todos os arquivos build/*.o, com base no que está em src/*.c

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

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

LIB_SRCS = $(wildcard src/*.c)
LIB_OBJS = $(patsubst src/%.c,build/%.o,$(LIB_SRCS))
STATIC_LIB = lib/libbioestat.a
SHARED_LIB = lib/libbioestat.$(SHARED_EXT)

all: bin/app_estatica bin/app_dinamica

# qualquer build/x.o vem de src/x.c
# Para eu lembrar: $@ é o alvo, $< é a primeira dependência
# Exemplo: o comando vira:
# gcc -Wall -Wextra -std=c11 -Iinclude -c src/epidemiologia.c -o build/epidemiologia.o
# O -fPIC gera código que não assume endereços fixos
build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

build/main.o: app/main.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@
	

# Biblioteca estatica aqui: é um "pacote" com todos os .o da lib (sem o main.o)
# Alvo: a biblioteca. Pré requisitos: LIB_OBJS (que sao os .o em build/)
$(STATIC_LIB): $(LIB_OBJS)
	mkdir -p lib
	ar rcs $@ $^

# Biblioteca dinamica: os mesmos .o, ligados num unico arquivo carregado na execucao
$(SHARED_LIB): $(LIB_OBJS)
	mkdir -p lib
	$(CC) $(SHARED_FLAGS) -o $@ $^

# O app e ligado com a biblioteca,
bin/app_estatica: build/main.o $(STATIC_LIB)
	mkdir -p bin
	$(CC) build/main.o $(STATIC_LIB) -o $@

# App dinamico: -L diz ONDE procurar, -l diz QUAL biblioteca, o rpath diz onde ela estara na EXECUCAO
bin/app_dinamica: build/main.o $(SHARED_LIB)
	mkdir -p bin
	$(CC) build/main.o -Llib -lbioestat $(RPATH) -o $@

clean:
	rm -rf build bin lib