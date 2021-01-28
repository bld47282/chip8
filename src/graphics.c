typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
} SDL_Handler;

SDL_Handler initialise_window() {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Handler handler;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL error: %s\n", SDL_GetError());
	} else {
		SDL_CreateWindowAndRenderer(SCREEN_WIDTH * 10, SCREEN_HEIGHT * 10, SDL_WINDOW_SHOWN, &window, &renderer);
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	handler.window = window;
	handler.renderer = renderer;
	
	return handler;
}

void draw(chip8 * ch8, SDL_Handler handler) {
	SDL_SetRenderDrawColor(handler.renderer, 0, 0, 0, 0);
	SDL_RenderClear(handler.renderer);
	SDL_SetRenderDrawColor(handler.renderer, 255, 255, 255, 255);

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			if (ch8->graphics_buffer[x][y] == 0xFFFFFFFF) {
				for (int i = x * 10; i < (x * 10 + 10); i++) {
					for (int j = y * 10; j < (y * 10 + 10); j++) {
						SDL_RenderDrawPoint(handler.renderer, i, j);
					}
				}
			}
		}
	}
	SDL_RenderPresent(handler.renderer);
	ch8->draw_flag = 0;
}
