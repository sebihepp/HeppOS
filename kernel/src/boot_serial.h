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

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <return_value.h>
#include <cpu/io.h>


enum BootSerialParity {
	BootSerialParityNone = 0,
	BootSerialParityOdd = 1,
	BootSerialParityEven = 3,
	BootSerialParityMark = 5,
	BootSerialParitySpace = 7,
	
};

enum BootSerialStopSize {
	BootSerialStopSize1 = 0,
	BootSerialStopSize2 = 1,
};

class BootSerial {
private:
	uint16_t port_;
	uint16_t baud_rate_;
	uint8_t char_size_;
	BootSerialStopSize stop_size_;
	BootSerialParity parity_;
	

public:	
	BootSerial();
	~BootSerial();
	
	ReturnValue Init(uint16_t port, uint32_t baud_rate, uint8_t char_size,
	                   BootSerialStopSize stop_size,
	                   BootSerialParity parity) __attribute__((nothrow));
	void Send(char data) __attribute__((nothrow));
	void Send(const char *string) __attribute__((nothrow));
	char Receive(void) __attribute__((nothrow));

	bool CheckSendBufferEmpty(void) __attribute__((nothrow));
	bool CheckReceiveBufferEmpty(void) __attribute__((nothrow));
};
