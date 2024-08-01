
#include "console8.hpp"


Console8::Console8() {
}

Console8::~Console8() {
}
	
uint8_t Console8::ConvertColor(uint32_t color) {
	uint8_t _red = (color >> 16) & 0xFF;
	uint8_t _green = (color >> 8) & 0xFF;
	uint8_t _blue = color & 0xFF;
	
	// Highest bits only
	_red >>= 5;
	_green >>= 5;
	_blue >>= 6;
	
	uint8_t _result = 0;
	_result = _blue;
	_result |= _green << 2;
	_result |= _red << 5;
	
	return _result;
}

void Console8::SetPixel(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= Width)
		return;
	if (y >= Height)
		return;
		
	volatile uint8_t *target = (uint8_t*)(((uintptr_t)Console::Framebuffer) + (y * Pitch) + (x));
	*target = ConvertColor(color);
}
