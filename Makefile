
# Makefile bem básico - versão para arquivos genéricos, podendo ser usado em todos os módulos
# OBS: não tem os .h aqui ainda, então mudanças nos .h ainda não vao ser vistas

# Todos os .c de src/ viram .o em build/ (descobertos automaticamente)
# wildcard - faz um ls src/*.c aqui, essa variável vira todos os src/*.c
# patsubst faz um map(), em que pega todos os *.c que tem em src e vira build/*.o, assim,
# essa variável tem todos os arquivos build/*.o, com base no que está em src/*.c

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

LIB_SRCS = $(wildcard src/*.c)   
LIB_OBJS = $(patsubst src/%.c,build/%.o,$(LIB_SRCS))
STATIC_LIB = lib/libbioestat.a

all: bin/app_estatica

# qualquer build/x.o vem de src/x.c
# Para eu lembrar: $@ é o alvo, $< é a primeira dependência
# Exemplo: o comando vira:
# gcc -Wall -Wextra -std=c11 -Iinclude -c src/epidemiologia.c -o build/epidemiologia.o
build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# fiz uma regra própria pois está na pasta app/
build/main.o: app/main.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Biblioteca estatica aqui: é um "pacote" com todos os .o da lib (sem o main.o)
# Alvo: a biblioteca. Pré requisitos: LIB_OBJS (que sao os .o em build/)
$(STATIC_LIB): $(LIB_OBJS)
	mkdir -p lib
	ar rcs $@ $^

# O app e ligado com a biblioteca,
bin/app_estatica: build/main.o $(STATIC_LIB)
	mkdir -p bin
	$(CC) build/main.o $(STATIC_LIB) -o $@


clean:
	rm -rf build bin lib