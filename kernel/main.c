
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

volatile uint16_t *videoMem = (uint16_t*)0xB8000;
uint8_t col = 0;
uint8_t row = 0;

void print(char *text) {
	while (*text != 0) {

		if (row > 24)
			return;
		
		uint16_t pos = row*80+col;
		switch (*text) {
			case '\n':
				col = 0;
				row++;
				break;
			default:
				col++;
				if (col > 79) {
					col = 0;
					row++;
				}
				videoMem[pos] = (videoMem[pos] & 0xFF00) | *text;
		}

		text++;
	}
}

void kmain(uint32_t magic, void *mbi) {

	print("HeppOS\n\nYou can now turn off the computer...\n");
	
	volatile uint16_t *test = (uint16_t*)0xB8000;
	
	return;

}

