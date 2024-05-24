
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

volatile uint16_t *videoMem = (uint16_t*)0xB8000;
uint8_t col = 0;
uint8_t row = 0;

extern "C" void kprint(const char *text) {
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

extern "C" void kmain(uint32_t magic, void *mbi) {

	kprint("HeppOS\n\n");
	if (magic != 0x36D76289) {
		kprint("Multiboot magic number - ERROR\n\n");
	} else {
		kprint("Multiboot magic number - OK\n\n");
	}
	
	return;

}
