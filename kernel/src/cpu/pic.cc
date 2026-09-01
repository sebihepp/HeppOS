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
#include <kstdlib/kstring.h>

uint8_t PIC::offset_ = 0;
uint64_t PIC::spurious_count_ = 0;
uint16_t PIC::mask_ = 0;


ReturnValue PIC::Init(uint8_t offset) {

	MaskAll();
	
	// Start Init
	outb(PIC_MASTER_COMMAND_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	outb(PIC_SLAVE_COMMAND_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);

	// ICW2 - Offset
	outb(PIC_MASTER_DATA_PORT, offset);
	outb(PIC_SLAVE_DATA_PORT, offset + 8);
	
	// ICW3
	outb(PIC_MASTER_DATA_PORT, 0x04);		//Set IRQ2 as Slave PIC
	outb(PIC_SLAVE_DATA_PORT, 0x02);		//Set Cascade Identity

	// ICW4
	outb(PIC_MASTER_DATA_PORT, PIC_ICW4_8086);
	outb(PIC_SLAVE_DATA_PORT, PIC_ICW4_8086);

	MaskAll();
	
	offset_ = offset;
	mask_ = 0xFFFF;
	
	return ReturnValueOk;
}

void PIC::SendEOI(uint8_t interrupt) {
		
	uint8_t irq = interrupt - offset_;
	
	if (irq >= GetIntLineCount())
		return;
	
	//Handle Spurious Interrupts
	if (irq == 7) {
		//Read ISR (In-Service Register)
		outb(PIC_MASTER_COMMAND_PORT, PIC_READ_ISR);
		uint8_t _Value = inb(PIC_MASTER_COMMAND_PORT);
		// Skip if it is a Spurious Int
		if ((_Value & 0x80) == 0) {
			spurious_count_++;
			return;
		}
	}
	if (irq == 15) {
		//Read ISR (In-Service Register)
		outb(PIC_SLAVE_COMMAND_PORT, PIC_READ_ISR);
		uint8_t _Value = inb(PIC_SLAVE_COMMAND_PORT);
		// Skip if it is a Spurious Int
		if ((_Value & 0x80) == 0) {
			spurious_count_++;
			outb(PIC_MASTER_COMMAND_PORT, PIC_EOI);
			return;
		}
	}
	
	// Handle normal Interrupts
	if (irq >= 8) {
		outb(PIC_SLAVE_COMMAND_PORT, PIC_EOI);
	}
	outb(PIC_MASTER_COMMAND_PORT, PIC_EOI);
}

void PIC::Mask(uint8_t irq) {
	uint16_t _Port = 0;
	uint8_t _Mask = 0;
	uint8_t _Value = 0;
	
	if (irq > 15)
		return;
	
	if (irq < 8) {
		_Port = PIC_MASTER_DATA_PORT;
		_Mask = 1 << irq;
	} else {
		_Port = PIC_SLAVE_DATA_PORT;
		_Mask = 1 << (irq - 8);
	}
	
	_Value = inb(_Port);
	_Value |= _Mask;
	outb(_Port, _Value);
	
	mask_ |= (1 << irq);
}

void PIC::Unmask(uint8_t irq) {
	uint16_t _Port = 0;
	uint8_t _Mask = 0;
	uint8_t _Value = 0;
	
	if (irq > 15)
		return;
	
	if (irq < 8) {
		_Port = PIC_MASTER_DATA_PORT;
		_Mask = 1 << irq;
	} else {
		_Port = PIC_SLAVE_DATA_PORT;
		_Mask = 1 << (irq - 8);
	}
	
	_Value = inb(_Port);
	_Value &= ~_Mask;
	outb(_Port, _Value);
	
	mask_ &= ~(1 << irq);
}

void PIC::MaskAll(void) {
	outb(PIC_MASTER_DATA_PORT, 0xFF);
	outb(PIC_SLAVE_DATA_PORT, 0xFF);
}

void PIC::UnmaskAll(void) {
	outb(PIC_MASTER_DATA_PORT, 0x00);
	outb(PIC_SLAVE_DATA_PORT, 0x00);
	mask_ = 0;
}

void PIC::RestoreMask(void) {
	outb(PIC_MASTER_DATA_PORT, (mask_ & 0xFF));
	outb(PIC_SLAVE_DATA_PORT, ((mask_ >> 8) & 0xFF));
}

void PIC::SetMask(uint16_t mask) {
	mask_ = mask;
	RestoreMask();
}

uint16_t PIC::GetMask(void) {
	return mask_;
}

uint8_t PIC::GetOffset(void) {
	return offset_;
}

uint8_t PIC::GetIntLineCount(void) {
	return 16;
}

uint64_t PIC::GetSpuriousCount(void) {
	return spurious_count_;
}

bool PIC::CheckSpurious(uint8_t interrupt) {
	
	uint8_t irq = interrupt - offset_;
	
	if (irq == 7) {
		//Read ISR (In-Service Register)
		outb(PIC_MASTER_COMMAND_PORT, PIC_READ_ISR);
		uint8_t _Value = inb(PIC_MASTER_COMMAND_PORT);
		//Check for Spurious
		if ((_Value & 0x80) == 0) {
			return true;
		}
	} else if (irq == 15) {
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

