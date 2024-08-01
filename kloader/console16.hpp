
#ifndef HEADER_CONSOLE16
#define HEADER_CONSOLE16

#include "console.hpp"

class Console16: public Console {
	
private:
	uint16_t ConvertColor(uint32_t color);
	
public:
	Console16();
	~Console16();
	
	void SetPixel(uint32_t x, uint32_t y, uint32_t color);
	
};


#endif
