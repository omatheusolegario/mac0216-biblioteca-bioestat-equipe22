
# Makefile bem básico - versão para arquivos genéricos, podendo ser usado em todos os módulos
# OBS: não tem os .h aqui ainda, então mudanças nos .h ainda não vao ser vistas
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# Todos os .c de src/ viram .o em build/ (descobertos automaticamente)
LIB_SRCS = $(wildcard src/*.c)
LIB_OBJS = $(patsubst src/%.c,build/%.o,$(LIB_SRCS))

all: bin/app

# Regra generica: qualquer build/X.o vem de src/X.c
build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/main.o: app/main.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

bin/app: build/main.o $(LIB_OBJS)
	mkdir -p bin
	$(CC) $^ -o $@

clean:
	rm -rf build bin