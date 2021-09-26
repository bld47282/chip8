#include <string.h>

#define MEMORY_SIZE 0xFFF
#define MEMORY_BUFFER 0x200
#define N_REGISTERS 0x10
#define STACK_DEPTH 0x10
#define FONT_SIZE 80
#define FONT_START_ADDRESS 0x50
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define NUM_KEYS 0x10

unsigned char fontset[FONT_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

typedef struct {
	unsigned char mem[MEMORY_SIZE];
	unsigned char v[N_REGISTERS];
	unsigned short i;
	unsigned short pc;
	unsigned short stack[STACK_DEPTH];
	unsigned char sp;
	unsigned short opcode;

	unsigned char delay_timer;
	unsigned char sound_timer;

	unsigned int graphics_buffer[SCREEN_WIDTH][SCREEN_HEIGHT];
	unsigned char keypad[NUM_KEYS];
	unsigned char draw_flag;
} chip8;

void fetch(chip8 *);
void execute(chip8 *);
void timer_update(chip8 *);

void initialise(chip8 *ch8) {
	// make everything 0;
	memset(ch8->mem, 0, sizeof(ch8->mem));
	memset(ch8->stack, 0, sizeof(ch8->stack));
	memset(ch8->v, 0, sizeof(ch8->v));
	memset(ch8->keypad, 0, sizeof(ch8->keypad));

	ch8->i = 0;
	ch8->sp = 0;
	ch8->opcode = 0;
	ch8->draw_flag = 0;

	memset(ch8->graphics_buffer, 0, sizeof(ch8->graphics_buffer));

	// set pc to first instruction;
	
	ch8->pc = MEMORY_BUFFER;

	// load font set
	for (int i = 0; i < FONT_SIZE; i++)
		ch8->mem[FONT_START_ADDRESS + i] = fontset[i];
}

void cycle(chip8 *ch8) {
        // fetch opcode
	fetch(ch8);

	if (ch8->opcode == 0x1228)
		exit(0);
	
	// execute opcode
	execute(ch8);
	
	// update timers
	timer_update(ch8);
	
	printf("Instruction: %x\n", ch8->opcode);
}

void load_rom(chip8 *ch8, char *filename) {
	FILE *rom = fopen(filename, "r");

	fread(&ch8->mem[MEMORY_BUFFER], 1, MEMORY_SIZE - MEMORY_BUFFER, rom);
}

void fetch(chip8 *ch8) {
	ch8->opcode = ch8->mem[ch8->pc] << 8 | ch8->mem[ch8->pc+1];

	ch8->pc += 2;
}

void execute(chip8 *ch8) {
	unsigned short check_initial = ch8->opcode & 0xF000;
	unsigned short check_final = ch8->opcode & 0x000F;
	unsigned short check_final_and_penultimate = ch8->opcode & 0x00FF;
	
	unsigned char x = (ch8->opcode >> 8) & 0x0F;
	unsigned char y = (ch8->opcode >> 4) & 0x0F;
	unsigned char kk = (ch8->opcode & 0x00FF);
	unsigned char n = (ch8->opcode & 0x000F);	
	unsigned short nnn = (ch8->opcode & 0x0FFF);


	switch(check_initial) {
		case 0x1000:
			// JP addr
			// Jump to location
			ch8->pc = nnn;
			break;

		case 0x2000:
			// CALL addr
			// Call subroutine
			ch8->sp++;
			ch8->stack[ch8->sp] = ch8->pc;
			ch8->pc = nnn;
			break;

		case 0x3000:
			// SE Vx, byte
			// Skip next instruction if Vx == byte
			if (ch8->v[x] == kk)
				ch8->pc += 2;
			break;

		case 0x4000:
			// SNE Vx, byte
			// Skip next instruction if Vx != byte
			if (ch8->v[x] != kk)
				ch8->pc += 2;
			break;

		case 0x5000:
			// SE Vx, Vy
			// Skip next instruction if Vx == Vy
			if (ch8->v[x] == ch8->v[y])
				ch8->pc += 2;
			break;
			
		case 0x6000:
			// LD Vx, byte
			// Set Vx == byte
			ch8->v[x] = kk;
			break;

		case 0x7000:
			// ADD Vx, byte
			// Set Vx += byte
			ch8->v[x] += kk;
			break;

		case 0x9000:
			// SNE Vx, Vy
			// Skip next instruction if Vx != Vy
			if (ch8->v[x] != ch8->v[y])
				ch8->pc += 2;
			break;

		case 0xA000:
			// LD I, addr
			// Set register I to addr
			ch8->i = nnn;
			break;

		case 0xB000:
			// JP V0, addr
			// Set PC to addr + V0
			ch8->pc = nnn + ch8->v[0];
			break;

		case 0xC000:
			// RND Vx, byte
			// Set Vx = random byte && kk
			ch8->v[x] = kk & (rand() % 256);
			break;

		case 0xD000: ;
			// DRW Vx, Vy, nibble
			// Draw n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
			unsigned char vx = ch8->v[x] % 64;
			unsigned char vy = ch8->v[y] % 32;
			ch8->v[0xF] = 0;

			unsigned char screen_byte;
			
			for (int row = 0; row < n; row++) {
				screen_byte = ch8->mem[ch8->i + row];
        			for (int col = 0; col < 8; col++) {
            				if ((screen_byte & (0x80 >> col)) != 0) {
                				if (ch8->graphics_buffer[vx + col][vy + row] == 0xFFFFFFFF) {
                    					ch8->v[0xF] = 1;
                				}		
                				ch8->graphics_buffer[vx + col][vy + row] ^= 0xFFFFFFFF;
            				}
        			}
    			}
			ch8->draw_flag = 1;
			break;

		case 0x8000:

			switch(check_final) {
				case 0x0000:
					// LD Vx, Vy
					// Set Vx = Vy
					ch8->v[x] = ch8->v[y];
					break;

				case 0x0001:
					// OR Vx, Vy
					// Set Vx = Vx || Vy
					ch8->v[x] = (ch8->v[x] | ch8->v[y]);
					break;

				case 0x0002:
					// AND Vx, Vy
					// Set Vx = Vx AND Vy
					ch8->v[x] = (ch8->v[x] & ch8->v[y]);
					break;

				case 0x0003:
					// XOR Vx, Vy
					// Set Vx = Vx XOR Vy
					ch8->v[x] = (ch8->v[x] ^ ch8->v[y]);
					break;

				case 0x0004: ;
					// ADD Vx, Vy
					// Set Vx = Vx + Vy, VF = carry
					unsigned short result = ch8->v[x] + ch8->v[y];
					if (result > 255)
						ch8->v[0xF] = 1;
					else
						ch8->v[0xF] = 0;

					ch8->v[x] = result;
					break;

				case 0x0005:
					// SUB Vx, Vy
					// Set Vx = Vx - Vy, VF = NOT borrow
					if (ch8->v[x] > ch8->v[y])
						ch8->v[0xF] = 1;
					else
						ch8->v[0xF] = 0;

					ch8->v[x] -= ch8->v[y];
					break;

				case 0x0006:
					// SHR Vx(, Vy)
					// Set Vx = Vx SHR 1	
					if ((ch8->v[x] & 0x01) == 1)
						ch8->v[0xF] = 1;
					else
						ch8->v[0xF] = 0;
					ch8->v[x] = (ch8->v[x]) / 2;
					break;

				case 0x0007:
					// SUBN Vx, Vy
					// Set Vx = Vy - Vx, VF = NOT borrow
					if (ch8->v[y] > ch8->v[x])
						ch8->v[0xF] = 1;
					else
						ch8->v[0xF] = 0;

					ch8->v[x] = ch8->v[y] - ch8->v[x];
					break;

				case 0x000E:
					// SHL Vx(, Vy)
					// Set Vx = Vx SHR 1	
					if ((ch8->v[x] & 0x80) == 0x80)
						ch8->v[0xF] = 1;
					else
						ch8->v[0xF] = 0;

					ch8->v[x] = (ch8->v[x]) * 2;
					break;

				default:
					printf("Instruction not implemented: %x\n", ch8->opcode);
			}
			break;
		case 0x0000:
			switch(check_final_and_penultimate) {
				case 0x00E0:
					// CLS
					// Clear the display.
					memset(ch8->graphics_buffer, 0, sizeof(ch8->graphics_buffer));
					break;

				case 0x00EE:
					// RET
					// Return from a subroutine.
					ch8->pc = ch8->stack[ch8->sp];
					ch8->sp--;
					break;

				default:
					printf("Instruction not implemented: %x\n", ch8->opcode);
			}
			break;
		case 0xE000:
			switch(check_final_and_penultimate) {
				case 0x00A1:
					// SKNP Vx
					// Skip the next instruction if the key in Vx is not pressed
					if (ch8->keypad[ch8->v[x]] == 0x00) {
						ch8->pc += 2;
					}
					break;

				case 0x009E:
					// SKP Vx
					// Skip the next instruction if the key in Vx is pressed
					if (ch8->keypad[ch8->v[x]] == 0xFF) {
						ch8->pc += 2;
					}
					break;

				default:
					printf("Instruction not implemented: %x\n", ch8->opcode);
			}
			break;
		case 0xF000:
			switch(check_final_and_penultimate) {
				case 0x0007:
					// LD Vx, DT
					// Set Vx = DT
					ch8->v[x] = ch8->delay_timer;
					break;

				case 0x000A:
					printf("Instruction not implemented: %x\n", ch8->opcode);
					break;

				case 0x0015:
					// LD DT, Vx
					// Set DT = Vx
					ch8->delay_timer = ch8->v[x];
					break;

				case 0x0018:
					// LD ST, Vx
					// Set ST = Vx
					ch8->sound_timer = ch8->v[x];
					break;

				case 0x001E:
					// ADD I, Vx
					// Set I = I + Vx
					ch8->i += ch8->v[x];
					break;

				case 0x0029:
					ch8->i = FONT_START_ADDRESS + (5 * ch8->v[x]);
					break;

				case 0x0033:
					ch8->mem[ch8->i+2] = (ch8->v[x] % 10);
					ch8->mem[ch8->i+1] = ((ch8->v[x] / 10) % 10);
					ch8->mem[ch8->i] = (((ch8->v[x] / 10) / 10) % 10);
					break;

				case 0x0055:
					for (int i = 0; i <= x; i++) {
						ch8->mem[ch8->i + i] = ch8->v[i];
					}
					break;

				case 0x0065:
					for (int i = 0; i <= x; i++) {
						ch8->v[i] = ch8->mem[ch8->i + i];
					}
					break;

				default:
					printf("Instruction not implemented: %x\n", ch8->opcode);
			}
			break;
		default:
			printf("Instruction not implemented: %x\n", ch8->opcode);
	}
}

void timer_update(chip8 *ch8) {
	if (ch8->delay_timer > 0)
		--ch8->delay_timer;
	if (ch8->sound_timer > 0)
		--ch8->sound_timer;
}
