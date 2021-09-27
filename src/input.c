void get_key_state(chip8 *ch8, SDL_Event event) {
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_1:
						ch8->keypad[0x1] = 0xFF;
						break;
					case SDL_SCANCODE_2:
						ch8->keypad[0x2] = 0xFF;
						break;
					case SDL_SCANCODE_3:
						ch8->keypad[0x3] = 0xFF;
						break;
					case SDL_SCANCODE_4:
						ch8->keypad[0xC] = 0xFF;
						break;
					case SDL_SCANCODE_Q:
						ch8->keypad[0x4] = 0xFF;
						break;
					case SDL_SCANCODE_W:
						ch8->keypad[0x5] = 0xFF;
						break;
					case SDL_SCANCODE_E:
						ch8->keypad[0x6] = 0xFF;
						break;
					case SDL_SCANCODE_R:
						ch8->keypad[0xD] = 0xFF;
						break;
					case SDL_SCANCODE_A:
						ch8->keypad[0x7] = 0xFF;
						break;
					case SDL_SCANCODE_S:
						ch8->keypad[0x8] = 0xFF;
						break;
					case SDL_SCANCODE_D:
						ch8->keypad[0x9] = 0xFF;
						break;
					case SDL_SCANCODE_F:
						ch8->keypad[0xE] = 0xFF;
						break;
					case SDL_SCANCODE_Z:
						ch8->keypad[0xA] = 0xFF;
						break;
					case SDL_SCANCODE_X:
						ch8->keypad[0x0] = 0xFF;
						break;
					case SDL_SCANCODE_C:
						ch8->keypad[0xB] = 0xFF;
						break;
					case SDL_SCANCODE_V:
						ch8->keypad[0xF] = 0xFF;
						break;
					case SDL_SCANCODE_SPACE:
						ch8->pause = 1;
						break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_1:
						ch8->keypad[0x1] = 0x00;
						break;
					case SDL_SCANCODE_2:
						ch8->keypad[0x2] = 0x00;
						break;
					case SDL_SCANCODE_3:
						ch8->keypad[0x3] = 0x00;
						break;
					case SDL_SCANCODE_4:
						ch8->keypad[0xC] = 0x00;
						break;
					case SDL_SCANCODE_Q:
						ch8->keypad[0x4] = 0x00;
						break;
					case SDL_SCANCODE_W:
						ch8->keypad[0x5] = 0x00;
						break;
					case SDL_SCANCODE_E:
						ch8->keypad[0x6] = 0x00;
						break;
					case SDL_SCANCODE_R:
						ch8->keypad[0xD] = 0x00;
						break;
					case SDL_SCANCODE_A:
						ch8->keypad[0x7] = 0x00;
						break;
					case SDL_SCANCODE_S:
						ch8->keypad[0x8] = 0x00;
						break;
					case SDL_SCANCODE_D:
						ch8->keypad[0x9] = 0x00;
						break;
					case SDL_SCANCODE_F:
						ch8->keypad[0xE] = 0x00;
						break;
					case SDL_SCANCODE_Z:
						ch8->keypad[0xA] = 0x00;
						break;
					case SDL_SCANCODE_X:
						ch8->keypad[0x0] = 0x00;
						break;
					case SDL_SCANCODE_C:
						ch8->keypad[0xB] = 0x00;
						break;
					case SDL_SCANCODE_V:
						ch8->keypad[0xF] = 0x00;
						break;
					case SDL_SCANCODE_SPACE:
						ch8->pause = 0;
						break;
				}
				break;
			default:
				break;
		}	
	}
}
