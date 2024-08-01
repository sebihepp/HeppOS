
#include "console24.hpp"


Console24::Console24() {
}

Console24::~Console24() {
}
	
void Console24::SetPixel(uint32_t x, uint32_t y, uint32_t color) {
	
	if (x >= Width)
		return;
	if (y >= Height)
		return;
	
	// Mask out the highest 8 bit
	color &= 0x00FFFFFF;
	
	volatile uint32_t *target = (uint32_t*)(((uintptr_t)Console::Framebuffer) + (y * Pitch) + (x * 3));
	// Mask out the highest 8 bit
	color &= 0x00FFFFFF;
	color |= *target & 0xFF000000;
	*target = color;
}
