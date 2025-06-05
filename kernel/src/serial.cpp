
#include <serial.h>



CSerial::CSerial() {
	mPort = 0;
	mBaudRate = 9600;
	mCharSize = 8;
	mStopSize = SERIAL_STOPSIZE_2;
	mParity = SERIAL_PARITY_NONE;
}

CSerial::~CSerial() {
	
}

	
ReturnValue_t CSerial::Init(uint16_t pPort, uint32_t pBaudRate, uint8_t pCharSize, SerialStopSize pStopSize, SerialParity pParity) {

	uint16_t _BaudRateDivisor = 115200 / pBaudRate;
	
	if (115200 % pBaudRate != 0) {
		return RETVAL_ERROR_GENERAL;
	}
	if (pCharSize < 5 || pCharSize > 8) {
		return RETVAL_ERROR_GENERAL;
	}
	
	mPort = pPort;
	mBaudRate = pBaudRate;
	mCharSize = pCharSize;
	mStopSize = pStopSize;
	mParity = pParity;
	
	
	outb(mPort + 3, 0x00);							//Reset DLAB to 0
	outb(mPort + 1, 0x00);							//Disable interrupts
	outb(mPort + 3, 0x80);							//Enable DLAB (Ports 0 and 1 are now for BaudRate divisor)
	outb(mPort + 0, _BaudRateDivisor & 0xFF);		//Set low Byte of BaudRate
	outb(mPort + 1, _BaudRateDivisor >> 8);			//Set high Byte of BaudRate
	outb(mPort + 3, ((pCharSize - 5) & 0x3) | ((mStopSize & 0x1) << 2) | ((mParity & 0x7) << 3) );		//Set DataSize, StopSize and Parity
	outb(mPort + 2, 0xC7);							//Enable FIFO, clear FIFO, 14 Byte FIFO
	
	//Set to Loopback mode for testing
	outb(mPort + 4, 0x13);
	//Send Test
	Send((char)0x5A);
	if (Receive() != 0x5A) {
		return RETVAL_ERROR_GENERAL;
	}
	//Disable loopback
	outb(mPort + 4, 0x0F);
	
	return RETVAL_OK;
}

void CSerial::Send(char pData) {
	//Wait for transmit FIFO to not be full
	while (CheckSendBufferEmpty() == false)
		;
	
	outb(mPort, pData);
}

void CSerial::Send(const char *pString) {
	uint32_t i = 0;
	while (pString[i] != 0){
		Send(pString[i]);
		++i;
	}
}

char CSerial::Receive(void) {
	while (CheckReceiveBufferEmpty() == false)
		;
	
	return inb(mPort);
}

bool CSerial::CheckSendBufferEmpty(void) {
	return ((inb(mPort + 5) & 0x20) == 0x20);
}

bool CSerial::CheckReceiveBufferEmpty(void) {
	return ((inb(mPort + 5) & 0x01) == 0x01);
}
