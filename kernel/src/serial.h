/* 
    Copyright (C) 2025  Sebastian Hepp aka sebihepp

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
	void Send(char pData) __attribute__ (( nothrow ));
	void Send(const char *pString) __attribute__ (( nothrow ));
	char Receive(void) __attribute__ (( nothrow ));

	bool CheckSendBufferEmpty(void) __attribute__ (( nothrow ));
	bool CheckReceiveBufferEmpty(void) __attribute__ (( nothrow ));
};

#endif
