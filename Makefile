CC=gcc
CFLAGS=-Wall

chip8: src/main.c
	$(CC) -o build/chip8 src/main.c
