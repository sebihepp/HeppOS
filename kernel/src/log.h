
#ifndef HEADER_LOG
#define HEADER_LOG

#include <stddef.h>
#include <stdint.h>

#include <retval.h>
#include <serial.h>


class CLog {
private:

	static CSerial mSerial;
	static bool mInitialized;
	
	CLog() = delete;
	~CLog() = delete;

public:
	
	static ReturnValue_t Init(void) __attribute__ (( nothrow ));
	
	static void Print(const char *pString) __attribute__ (( nothrow ));
	static void PrintF(const char *pString, ...) __attribute__ (( nothrow ));
	
};

#endif
