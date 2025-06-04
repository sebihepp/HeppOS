
#ifndef HEADER_SERIAL
#define HEADER_SERIAL

#include <retval.h>
#include <stddef.h>
#include <stdint.h>
#include <cpu/io.h>


enum SerialParity {
	SERIAL_PARITY_NONE = 0,
	SERIAL_PARITY_ODD = 1,
	SERIAL_PARITY_EVEN = 3,
	SERIAL_PARITY_MARK = 5,
	SERIAL_PARITY_SPACE = 7,
	
};

enum SerialStopSize {
	SERIAL_STOPSIZE_1 = 0,
	SERIAL_STOPSIZE_2 = 1,
};

class CSerial {
private:
	uint16_t mPort;
	uint16_t mBaudRate;
	uint8_t mCharSize;
	SerialStopSize mStopSize;
	SerialParity mParity;
	


public:	
	CSerial();
	~CSerial();
	
	ReturnValue_t Init(uint16_t pPort, uint32_t pBaudRate, uint8_t pCharSize, SerialStopSize pStopSize, SerialParity pParity) __attribute__ (( nothrow ));
	void Send(const char pData) __attribute__ (( nothrow ));
	void Send(const char *pString) __attribute__ (( nothrow ));
	char Receive(void) __attribute__ (( nothrow ));

};

#endif
