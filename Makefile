
# Makefile bem básico - to aprendendo a fazer

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

all: bin/app

build/epidemiologia.o: src/epidemiologia.c include/epidemiologia.h include/bioestat_erro.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/epidemiologia.c -o build/epidemiologia.o

build/bioestat_erro.o: src/bioestat_erro.c include/bioestat_erro.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/bioestat_erro.c -o build/bioestat_erro.o

build/main.o: app/main.c include/epidemiologia.h include/bioestat_erro.h
	mkdir -p build
	$(CC) $(CFLAGS) -c app/main.c -o build/main.o

bin/app: build/main.o build/epidemiologia.o build/bioestat_erro.o
	mkdir -p bin
	$(CC) build/main.o build/epidemiologia.o build/bioestat_erro.o -o bin/app

clean:
	rm -rf build bin