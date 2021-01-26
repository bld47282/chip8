#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip8.c"
#include "graphics.c"
#include "audio.c"
#include "input.c"

int main(int argc, char *argv[]) {
	char *filename;
	chip8 ch8;

	if (argc != 2) {
		printf("Usage: chip8 [filename]\n");
	        exit(0);
	}
	else {
		filename = argv[1];
	}
	initialise(&ch8);

	load_rom(&ch8, filename);

	while (1) {
		cycle(&ch8);

		if (ch8.draw_flag == 1)
			draw(&ch8);

		get_key_state(&ch8);
	}
	return 1;
}
