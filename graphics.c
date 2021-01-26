void draw(chip8 * ch8) {
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			if (ch8->graphics_buffer[x][y] == 1) {
				printf("X");
			} else {
				printf("-");
			}
		}
		printf("\n");
	}
	printf("----------------------------------------------------------------\n");
	ch8->draw_flag = 0;
}
