
#ifndef HEADER_CONSOLE32
#define HEADER_CONSOLE32

#include "console.hpp"

class Console32: public Console {
	
public:
	Console32();
	~Console32();
	
	void SetPixel(uint32_t x, uint32_t y, uint32_t color);
	
};


#endif
