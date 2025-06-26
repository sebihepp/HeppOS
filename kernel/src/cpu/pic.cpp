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

#include <cpu/pic.h>
#include <cpu/io.h>

uint8_t CPIC::mOffset = 0;
uint64_t CPIC::mSpuriousCount = 0;
uint16_t CPIC::mMask = 0;

// For quick testing - needs to be put in a string.h or something similar
char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);
char *htoa(uint64_t num, char* str);


ReturnValue_t CPIC::Init(uint8_t pOffset) {

	MaskAll();
	
	// Start Init
	outb(PIC_MASTER_COMMAND_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	outb(PIC_SLAVE_COMMAND_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);

	// ICW2 - Offset
	outb(PIC_MASTER_DATA_PORT, pOffset);
	outb(PIC_SLAVE_DATA_PORT, pOffset + 8);
	
	// ICW3
	outb(PIC_MASTER_DATA_PORT, 0x04);		//Set IRQ2 as Slave PIC
	outb(PIC_SLAVE_DATA_PORT, 0x02);		//Set Cascade Identity

	// ICW4
	outb(PIC_MASTER_DATA_PORT, PIC_ICW4_8086);
	outb(PIC_SLAVE_DATA_PORT, PIC_ICW4_8086);

	MaskAll();
	
	mOffset = pOffset;
	mMask = 0xFFFF;
	
	return RETVAL_OK;
}

void CPIC::SendEOI(uint8_t pInt) {
		
	uint8_t _Int = pInt - mOffset;
	
	if (_Int >= GetIntLineCount())
		return;
	
	//Handle Spurious Interrupts
	if (_Int == 7) {
		//Read ISR (In-Service Register)
		outb(PIC_MASTER_COMMAND_PORT, PIC_READ_ISR);
		uint8_t _Value = inb(PIC_MASTER_COMMAND_PORT);
		// Skip if it is a Spurious Int
		if ((_Value & 0x80) == 0) {
			mSpuriousCount++;
			return;
		}
	}
	if (_Int == 15) {
		//Read ISR (In-Service Register)
		outb(PIC_SLAVE_COMMAND_PORT, PIC_READ_ISR);
		uint8_t _Value = inb(PIC_SLAVE_COMMAND_PORT);
		// Skip if it is a Spurious Int
		if ((_Value & 0x80) == 0) {
			mSpuriousCount++;
			outb(PIC_MASTER_COMMAND_PORT, PIC_EOI);
			return;
		}
	}
	
	// Handle normal Interrupts
	if (_Int >= 8) {
		outb(PIC_SLAVE_COMMAND_PORT, PIC_EOI);
	}
	outb(PIC_MASTER_COMMAND_PORT, PIC_EOI);
}

void CPIC::Mask(uint8_t pIRQ) {
	uint16_t _Port = 0;
	uint8_t _Mask = 0;
	uint8_t _Value = 0;
	
	if (pIRQ > 15)
		return;
	
	if (pIRQ < 8) {
		_Port = PIC_MASTER_DATA_PORT;
		_Mask = 1 << pIRQ;
	} else {
		_Port = PIC_SLAVE_DATA_PORT;
		_Mask = 1 << (pIRQ - 8);
	}
	
	_Value = inb(_Port);
	_Value |= _Mask;
	outb(_Port, _Value);
	
	mMask |= (1 << pIRQ);
}

void CPIC::Unmask(uint8_t pIRQ) {
	uint16_t _Port = 0;
	uint8_t _Mask = 0;
	uint8_t _Value = 0;
	
	if (pIRQ > 15)
		return;
	
	if (pIRQ < 8) {
		_Port = PIC_MASTER_DATA_PORT;
		_Mask = 1 << pIRQ;
	} else {
		_Port = PIC_SLAVE_DATA_PORT;
		_Mask = 1 << (pIRQ - 8);
	}
	
	_Value = inb(_Port);
	_Value &= ~_Mask;
	outb(_Port, _Value);
	
	mMask &= ~(1 << pIRQ);
}

void CPIC::MaskAll(void) {
	outb(PIC_MASTER_DATA_PORT, 0xFF);
	outb(PIC_SLAVE_DATA_PORT, 0xFF);
}

void CPIC::UnmaskAll(void) {
	outb(PIC_MASTER_DATA_PORT, 0x00);
	outb(PIC_SLAVE_DATA_PORT, 0x00);
	mMask = 0;
}

void CPIC::RestoreMask(void) {
	outb(PIC_MASTER_DATA_PORT, (mMask & 0xFF));
	outb(PIC_SLAVE_DATA_PORT, ((mMask >> 8) & 0xFF));
}

void CPIC::SetMask(uint16_t pMask) {
	mMask = pMask;
	RestoreMask();
}

uint16_t CPIC::GetMask(void) {
	return mMask;
}

uint8_t CPIC::GetOffset(void) {
	return mOffset;
}

uint8_t CPIC::GetIntLineCount(void) {
	return 16;
}

uint64_t CPIC::GetSpuriousCount(void) {
	return mSpuriousCount;
}

bool CPIC::CheckSpurious(uint8_t pInt) {
	
	uint8_t _Int = pInt - mOffset;
	
	if (_Int == 7) {
		//Read ISR (In-Service Register)
		outb(PIC_MASTER_COMMAND_PORT, PIC_READ_ISR);
		uint8_t _Value = inb(PIC_MASTER_COMMAND_PORT);
		//Check for Spurious
		if ((_Value & 0x80) == 0) {
			return true;
		}
	} else if (_Int == 15) {
		//Read ISR (In-Service Register)
		outb(PIC_SLAVE_COMMAND_PORT, PIC_READ_ISR);
		uint8_t _Value = inb(PIC_SLAVE_COMMAND_PORT);
		//Check for Spurious
		if ((_Value & 0x80) == 0) {
			return true;
		}
	}
	return false;
}

