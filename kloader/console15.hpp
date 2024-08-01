
#ifndef HEADER_CONSOLE15
#define HEADER_CONSOLE15

#include "console.hpp"

class Console15: public Console {
	
private:
	uint16_t ConvertColor(uint32_t color);
	
public:
	Console15();
	~Console15();
	
	void SetPixel(uint32_t x, uint32_t y, uint32_t color);
	
};


#endif
