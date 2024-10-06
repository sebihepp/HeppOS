
#include <cpu/interrupt.h>

#include <video/console.h>
#include <cpu/gdt.h>
#include <cstub.h>

// For quick testing - needs to be put in a string.h or something similar
char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);
char *htoa(uint64_t num, char* str);


extern "C" void isr_wrapper(void);
extern "C" void isr_wrapper_128(void);
extern "C" void exception_wrapper_error(void);
extern "C" void exception_wrapper(void);
extern "C" void isr_handler(uint64_t pInt, uint64_t *pState);
extern "C" void exception_handler(uint64_t pInt, uint64_t *pState);

extern "C" ISRHandler_t gISRHandlerAddressTable[256];


IDTEntry_t Interrupt::mIDT[256];
IDTD_t Interrupt::mIDTD;
	
retval_t Interrupt::Init(void) {

	memset(mIDT, 0, sizeof(mIDT));
	memset(&mIDTD, 0, sizeof(mIDTD));
	
	
	
	// Exceptions
	for (uint64_t i = 0; i < 32; i++) {
		if (gISRHandlerAddressTable[i] != NULL) {
			RegisterException(i, gISRHandlerAddressTable[i]);
		}
	}

/* 	RegisterException(0, &exception_wrapper);
	RegisterException(1, &exception_wrapper);
	RegisterException(2, &exception_wrapper);
	RegisterException(3, &exception_wrapper);
	RegisterException(4, &exception_wrapper);
	RegisterException(5, &exception_wrapper);
	RegisterException(6, &exception_wrapper);
	RegisterException(7, &exception_wrapper);
	RegisterException(8, &exception_wrapper_error);
	RegisterException(9, &exception_wrapper);
	RegisterException(10, &exception_wrapper_error);
	RegisterException(11, &exception_wrapper_error);
	RegisterException(12, &exception_wrapper_error);
	RegisterException(13, &exception_wrapper_error);
	RegisterException(14, &exception_wrapper_error);
	RegisterException(15, &exception_wrapper);
	RegisterException(16, &exception_wrapper);
	RegisterException(17, &exception_wrapper_error);
	RegisterException(18, &exception_wrapper);
	RegisterException(19, &exception_wrapper);
	RegisterException(20, &exception_wrapper);
	RegisterException(21, &exception_wrapper);
	RegisterException(22, &exception_wrapper);
	RegisterException(23, &exception_wrapper);
	RegisterException(24, &exception_wrapper);
	RegisterException(25, &exception_wrapper);
	RegisterException(26, &exception_wrapper);
	RegisterException(27, &exception_wrapper);
	RegisterException(28, &exception_wrapper);
	RegisterException(29, &exception_wrapper);
	RegisterException(30, &exception_wrapper_error);
	RegisterException(31, &exception_wrapper); */
	
	RegisterHandler(128, gISRHandlerAddressTable[128]);
	
	
	// IDTD
	mIDTD.size = sizeof(mIDT) - 1;
	mIDTD.offset = (uint64_t)&mIDT;
	
	return RETVAL_OK;
}

void Interrupt::LoadIDT(void) {
	
	asm volatile (
		"lidt %0;\n"
		:
		: "m" (mIDTD)
		:
	);
}

void Interrupt::RegisterHandler(uint8_t pIndex, ISRHandler_t pHandler) {
	mIDT[pIndex].offset_l = (((uint64_t)pHandler) >> 0) & 0xFFFF;
	mIDT[pIndex].segment = GDT::GetSelector(GDT_CODE64_SEL);
	mIDT[pIndex].ist = 0x0;
	mIDT[pIndex].type = IDT_TYPE_GATE;
	mIDT[pIndex].dpl = IDT_DPL0;
	mIDT[pIndex].present = IDT_PRESENT;
	mIDT[pIndex].offset_m = (((uint64_t)pHandler) >> 16) & 0xFFFF;
	mIDT[pIndex].offset_h = (((uint64_t)pHandler) >> 32) & 0xFFFFFFFF;
}

void Interrupt::RegisterException(uint8_t pIndex, ISRHandler_t pHandler) {
	mIDT[pIndex].offset_l = (((uint64_t)pHandler) >> 0) & 0xFFFF;
	mIDT[pIndex].segment = GDT::GetSelector(GDT_CODE64_SEL);
	mIDT[pIndex].ist = 0x0;
	mIDT[pIndex].type = IDT_TYPE_TRAP;
	mIDT[pIndex].dpl = IDT_DPL0;
	mIDT[pIndex].present = IDT_PRESENT;
	mIDT[pIndex].offset_m = (((uint64_t)pHandler) >> 16) & 0xFFFF;
	mIDT[pIndex].offset_h = (((uint64_t)pHandler) >> 32) & 0xFFFFFFFF;
}

extern "C" void isr_handler(uint64_t pInt, uint64_t *pState) {

	char _TempString[16];
	Console::Print("Interrupt ");
	Console::Print(utoa(pInt, _TempString, 10));
	Console::Print(" occured!");
	
}

extern "C" void exception_handler(uint64_t pInt, uint64_t *pState) {
	
	char _TempString[16];
	Console::SetTitleText("HeppOS - Exception Handler");
	Console::SetBGColor(0x000000AA);
	Console::SetFGColor(0x0000FFFF);
	Console::Clear();
	Console::Print("ERROR - Exception ");
	Console::Print(utoa(pInt, _TempString, 10));
	Console::Print(" occured!\n");

	
	asm volatile (
		"cli;\n"
		"1:\n"
		"hlt;\n"
		"jmp 1b;\n"
	);
	
}
