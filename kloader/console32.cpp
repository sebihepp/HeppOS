
#include "console32.hpp"


Console32::Console32() {
}

Console32::~Console32() {
}
	
void Console32::SetPixel(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= Width)
		return;
	if (y >= Height)
		return;
		
	volatile uint32_t *target = (uint32_t*)(((uintptr_t)Console::Framebuffer) + (y * Pitch) + (x * 4));
	*target = color;
}
