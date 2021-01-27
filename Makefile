CC=gcc
CFLAGS=-Wall
CC_SDL=-lSDL2

chip8: src/main.c
	$(CC) -o build/chip8 src/main.c $(CC_SDL)
