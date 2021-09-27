#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "chip8.c"
#include "graphics.c"
#include "audio.c"
#include "input.c"

int main(int argc, char *argv[]) {
	// Initialise variables
	char *filename;
	chip8 ch8;

	// Initialise SDL objects
	SDL_Handler handler = initialise_window();
	SDL_Event event;
	
	// Handle arguments passed
	if (argc != 2) {
		printf("Usage: chip8 [filename]\n");
	        exit(0);
	}
	else {
		filename = argv[1];
	}
	
	// Initialise a CHIP-8 instance
	initialise(&ch8);
	
	// Load ROM file into CHIP-8 memory
	load_rom(&ch8, filename);


	// Main loop: run the CHIP-8 main loop, draw if necessary, check for input, delay
	while (1) {
		if (ch8.pause == 0) {
			cycle(&ch8);	

			if (ch8.draw_flag == 1)
				draw(&ch8, handler);

		}

		get_key_state(&ch8, event);

		SDL_Delay(2);
	}

	// Clean up and return
	destroy_handler(handler);
	return 0;
}
