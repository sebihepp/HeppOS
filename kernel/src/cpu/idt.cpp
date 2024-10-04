
#include <cpu/idt.h>

#include <video/console.h>
#include <cpu/gdt.h>
#include <cstub.h>


extern "C" void isr_wrapper(void);
extern "C" void isr_wrapper_error(void);
extern "C" void isr_handler(void);


IDTEntry_t IDT::mIDT[256];
IDTD_t IDT::mIDTD;
	
retval_t IDT::Init(void) {

	memset(mIDT, 0, sizeof(mIDT));
	memset(&mIDTD, 0, sizeof(mIDTD));
	
	
	// Exceptions	
	RegisterException(0, &isr_wrapper);
	RegisterException(1, &isr_wrapper);
	RegisterException(2, &isr_wrapper);
	RegisterException(3, &isr_wrapper);
	RegisterException(4, &isr_wrapper);
	RegisterException(5, &isr_wrapper);
	RegisterException(6, &isr_wrapper);
	RegisterException(7, &isr_wrapper);
	RegisterException(8, &isr_wrapper_error);
	RegisterException(9, &isr_wrapper);
	RegisterException(10, &isr_wrapper_error);
	RegisterException(11, &isr_wrapper_error);
	RegisterException(12, &isr_wrapper_error);
	RegisterException(13, &isr_wrapper_error);
	RegisterException(14, &isr_wrapper_error);
	RegisterException(15, &isr_wrapper);
	RegisterException(16, &isr_wrapper);
	RegisterException(17, &isr_wrapper_error);
	RegisterException(18, &isr_wrapper);
	RegisterException(19, &isr_wrapper);
	RegisterException(20, &isr_wrapper);
	RegisterException(21, &isr_wrapper);
	RegisterException(22, &isr_wrapper);
	RegisterException(23, &isr_wrapper);
	RegisterException(24, &isr_wrapper);
	RegisterException(25, &isr_wrapper);
	RegisterException(26, &isr_wrapper);
	RegisterException(27, &isr_wrapper);
	RegisterException(28, &isr_wrapper);
	RegisterException(29, &isr_wrapper);
	RegisterException(30, &isr_wrapper_error);
	RegisterException(31, &isr_wrapper);
	
	// IDTD
	mIDTD.size = sizeof(mIDT) - 1;
	mIDTD.offset = (uint64_t)&mIDT;
	
	return RETVAL_OK;
}

void IDT::LoadIDT(void) {
	
	asm volatile (
		"lidt %0;\n"
		:
		: "m" (mIDTD)
		:
	);
}

extern "C" void isr_handler(void) {
	
	
	Console::SetTitleText("HeppOS - Exception Handler");
	Console::SetBGColor(0x000000AA);
	Console::SetFGColor(0x0000FFFF);
	Console::Clear();
	Console::Print("ERROR - An Exception has occured!\n");
	
	asm volatile (
		"cli;\n"
		"1:\n"
		"hlt;\n"
		"jmp 1b;\n"
	);
}

void IDT::RegisterHandler(uint8_t pIndex, ISRHandler_t pHandler) {
	mIDT[pIndex].offset_l = (((uint64_t)pHandler) >> 0) & 0xFFFF;
	mIDT[pIndex].segment = GDT::GetSelector(GDT_CODE64_SEL);
	mIDT[pIndex].ist = 0x0;
	mIDT[pIndex].type = IDT_TYPE_GATE;
	mIDT[pIndex].dpl = IDT_DPL0;
	mIDT[pIndex].present = IDT_PRESENT;
	mIDT[pIndex].offset_m = (((uint64_t)pHandler) >> 16) & 0xFFFF;
	mIDT[pIndex].offset_h = (((uint64_t)pHandler) >> 32) & 0xFFFFFFFF;
}

void IDT::RegisterException(uint8_t pIndex, ISRHandler_t pHandler) {
	mIDT[pIndex].offset_l = (((uint64_t)pHandler) >> 0) & 0xFFFF;
	mIDT[pIndex].segment = GDT::GetSelector(GDT_CODE64_SEL);
	mIDT[pIndex].ist = 0x0;
	mIDT[pIndex].type = IDT_TYPE_TRAP;
	mIDT[pIndex].dpl = IDT_DPL0;
	mIDT[pIndex].present = IDT_PRESENT;
	mIDT[pIndex].offset_m = (((uint64_t)pHandler) >> 16) & 0xFFFF;
	mIDT[pIndex].offset_h = (((uint64_t)pHandler) >> 32) & 0xFFFFFFFF;
}
