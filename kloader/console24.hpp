
#ifndef HEADER_CONSOLE24
#define HEADER_CONSOLE24

#include "console.hpp"

class Console24: public Console {
	
private:
	
public:
	Console24();
	~Console24();
	
	void SetPixel(uint32_t x, uint32_t y, uint32_t color);
	
};


#endif
