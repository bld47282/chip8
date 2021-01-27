#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

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

	// SDL Initialisation

	SDL_Window * window = NULL;
	SDL_Surface* screen = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL error: %s\n", SDL_GetError());
	} else {
		window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
				SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	}
	if (window == NULL) {
		printf("Window not created: %s\n", SDL_GetError());
	} else {
		screen = SDL_GetWindowSurface(window);
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
		SDL_UpdateWindowSurface(window);
		SDL_Delay(2000);
	}
	
	// Main loop
	
	while (1) {
		cycle(&ch8);

		if (ch8.draw_flag == 1)
			draw(&ch8);

		get_key_state(&ch8);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
