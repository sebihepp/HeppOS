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

#include <boot_serial.h>


BootSerial::BootSerial() {
	port_ = 0;
	baud_rate_ = 9600;
	char_size_ = 8;
	stop_size_ = BootSerialStopSize1;
	parity_ = BootSerialParityNone;
}

BootSerial::~BootSerial() {
	
}

	
ReturnValue BootSerial::Init(uint16_t port, uint32_t baud_rate, 
	uint8_t char_size, BootSerialStopSize stop_size, BootSerialParity parity) {

	uint16_t baud_rate_divisor = 115200 / baud_rate;
	
	if (115200 % baud_rate != 0) {
		return ReturnValueErrorGeneral;
	}
	if (char_size < 5 || char_size > 8) {
		return ReturnValueErrorGeneral;
	}
	
	port_ = port;
	baud_rate_ = baud_rate;
	char_size_ = char_size;
	stop_size_ = stop_size;
	parity_ = parity;
	
	
	outb(port_ + 3, 0x00);	//Reset DLAB to 0
	outb(port_ + 1, 0x00);	//Disable interrupts
	outb(port_ + 3, 0x80);	//Enable DLAB (Ports 0 and 1 are now for BaudRate divisor)
	outb(port_ + 0, baud_rate_divisor & 0xFF);	//Set low Byte of BaudRate
	outb(port_ + 1, baud_rate_divisor >> 8);	//Set high Byte of BaudRate
	outb(port_ + 3, ((char_size_ - 5) & 0x3) | ((stop_size_ & 0x1) << 2) | 
		((parity_ & 0x7) << 3) );		//Set DataSize, StopSize and Parity
	outb(port_ + 2, 0xC7);	//Enable FIFO, clear FIFO, 14 Byte FIFO
	
	//Set to Loopback mode for testing
	outb(port_ + 4, 0x13);
	//Send Test
	Send((char)0x5A);
	if (Receive() != 0x5A) {
		return ReturnValueErrorGeneral;
	}
	//Disable loopback
	outb(port_ + 4, 0x0F);
	
	return ReturnValueOk;
}

void BootSerial::Send(char data) {
	//Wait for transmit FIFO to not be full
	while (CheckSendBufferEmpty() == false)
		;
	
	outb(port_, data);
}

void BootSerial::Send(const char *string) {
	uint32_t i = 0;
	while (string[i] != 0){
		Send(string[i]);
		++i;
	}
}

char BootSerial::Receive(void) {
	while (CheckReceiveBufferEmpty() == false)
		;
	
	return inb(port_);
}

bool BootSerial::CheckSendBufferEmpty(void) {
	return ((inb(port_ + 5) & 0x20) == 0x20);
}

bool BootSerial::CheckReceiveBufferEmpty(void) {
	return ((inb(port_ + 5) & 0x01) == 0x01);
}
