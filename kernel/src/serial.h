
#ifndef HEADER_SERIAL
#define HEADER_SERIAL

#include "retval.h"


class CSerial {
private:



public:	
	CSerial();
	~CSerial();
	
	ReturnValue_t Init(void) __attribute__ (( nothrow ));
	
};

#endif
