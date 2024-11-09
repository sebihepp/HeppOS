
#include <cpu/pic.h>
#include <cpu/io.h>

#include <video/console.h>


uint8_t PIC::mOffset = 0;
uint64_t PIC::mSpuriousCount = 0;

// For quick testing - needs to be put in a string.h or something similar
char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);
char *htoa(uint64_t num, char* str);


ReturnValue_t PIC::Init(uint8_t pOffset) {

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
	
	return RETVAL_OK;
}

void PIC::SendEOI(uint8_t pIntLine) {
		
/*	char _TempString[20];
	
 	Console::Print("called SendEOI with pIntLine=");
	Console::Print(utoa(pIntLine, _TempString, 10));
	Console::Print("\n"); */
	
	if (pIntLine >= GetIntLineCount())
		return;
	
	//Handle Spurious Interrupts
	if (pIntLine == 7) {
		//Read ISR (In-Service Register)
		outb(PIC_MASTER_COMMAND_PORT, PIC_READ_ISR);
		uint8_t _Value = inb(PIC_MASTER_COMMAND_PORT);
		// Skip if it is a Spurious Int
		if ((_Value & 0x80) == 0) {
			mSpuriousCount++;
			return;
		}
	}
	if (pIntLine == 15) {
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
	if (pIntLine >= 8) {
		outb(PIC_SLAVE_COMMAND_PORT, PIC_EOI);
	}
	outb(PIC_MASTER_COMMAND_PORT, PIC_EOI);
}

void PIC::Mask(uint8_t pIntLine) {
	uint16_t _Port = 0;
	uint8_t _Mask = 0;
	uint8_t _Value = 0;
	
	if (pIntLine > 15)
		return;
	
	if (pIntLine < 8) {
		_Port = PIC_MASTER_DATA_PORT;
		_Mask = 1 << pIntLine;
	} else {
		_Port = PIC_SLAVE_DATA_PORT;
		_Mask = 1 << (pIntLine - 8);
	}
	
	_Value = inb(_Port);
	_Value |= _Mask;
	outb(_Port, _Value);
}

void PIC::Unmask(uint8_t pIntLine) {
	uint16_t _Port = 0;
	uint8_t _Mask = 0;
	uint8_t _Value = 0;
	
	if (pIntLine > 15)
		return;
	
	if (pIntLine < 8) {
		_Port = PIC_MASTER_DATA_PORT;
		_Mask = 1 << pIntLine;
	} else {
		_Port = PIC_SLAVE_DATA_PORT;
		_Mask = 1 << (pIntLine - 8);
	}
	
	_Value = inb(_Port);
	_Value &= ~_Mask;
	outb(_Port, _Value);
}

void PIC::MaskAll(void) {
	outb(PIC_MASTER_DATA_PORT, 0xFF);
	outb(PIC_SLAVE_DATA_PORT, 0xFF);
}

void PIC::UnmaskAll(void) {
	outb(PIC_MASTER_DATA_PORT, 0x00);
	outb(PIC_SLAVE_DATA_PORT, 0x00);	
}

uint8_t PIC::GetOffset(void) {
	return mOffset;
}

uint8_t PIC::GetIntLineCount(void) {
	return 16;
}

uint64_t PIC::GetSpuriousCount(void) {
	return mSpuriousCount;
}
