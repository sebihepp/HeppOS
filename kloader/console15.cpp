
#include "console15.hpp"


Console15::Console15() {
}

Console15::~Console15() {
}

uint16_t Console15::ConvertColor(uint32_t color) {
	
	uint8_t _red = (color >> 16) & 0xFF;
	uint8_t _green = (color >> 8) & 0xFF;
	uint8_t _blue = color & 0xFF;
	
	// Highest bits only
	_red >>= 3;
	_green >>= 3;
	_blue >>= 3;
	
	uint16_t _result = 0;
	_result |= _blue;
	_result |= _green << 5;
	_result |= _red  << 10;
	
	return _result;
}


void Console15::SetPixel(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= Width)
		return;
	if (y >= Height)
		return;
		
	volatile uint16_t *target = (uint16_t*)(((uintptr_t)Console::Framebuffer) + (y * Pitch) + (x * 2));
	*target = ConvertColor(color);
}
