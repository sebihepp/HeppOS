
#ifndef HEADER_CONSOLE8
#define HEADER_CONSOLE8

#include "console.hpp"

class Console8: public Console {
	
private:
	uint8_t ConvertColor(uint32_t color);
	
public:
	Console8();
	~Console8();
	
	void SetPixel(uint32_t x, uint32_t y, uint32_t color);
	
};


#endif
