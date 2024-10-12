
#include <cpu/interrupt.h>

#include <video/console.h>
#include <cpu/gdt.h>
#include <cstub.h>
#include <cpu/io.h>


// For quick testing - needs to be put in a string.h or something similar
char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);
char *htoa(uint64_t num, char* str);


extern "C" void *gISRHandlerAddressTable[INTERRUPT_MAX_COUNT];


IDTEntry_t Interrupt::mIDT[INTERRUPT_MAX_COUNT];
IDTD_t Interrupt::mIDTD;

uint64_t Interrupt::mInterruptCount[INTERRUPT_MAX_COUNT];

retval_t Interrupt::Init(void) {

	memset(mIDT, 0, sizeof(mIDT));
	memset(&mIDTD, 0, sizeof(mIDTD));
	memset(mInterruptCount, 0, sizeof(mInterruptCount));
	
	
	// Exceptions
	for (uint64_t i = 0; i < 32; i++) {
		if (gISRHandlerAddressTable[i] != NULL) {
			SetIDTEntry(i, gISRHandlerAddressTable[i], IDT_TYPE_TRAP);
		}
	}
	
	// PIC
	for (uint64_t i = 32; i < 48; i++) {
		if (gISRHandlerAddressTable[i] != NULL) {
			SetIDTEntry(i, gISRHandlerAddressTable[i], IDT_TYPE_GATE);
		}
	}
	
	
	// SysCall
	SetIDTEntry(128, gISRHandlerAddressTable[128], IDT_TYPE_GATE);
	
	
	// IDTD
	mIDTD.size = sizeof(mIDT) - 1;
	mIDTD.offset = reinterpret_cast<uint64_t>(&mIDT);
	
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

void Interrupt::EnableInterrupts(void) {
	asm volatile ("sti;\n");
}

void Interrupt::DisableInterrupts(void) {
	asm volatile ("cli;\n");
}

void Interrupt::RegisterHandler(uint8_t pIndex, ISRHandler_t pHandler) {
	// ToDo: Implement
}

void Interrupt::SetIDTEntry(uint8_t pIndex, void *pHandler, uint8_t pType) {
	mIDT[pIndex].offset_l = (reinterpret_cast<uint64_t>(pHandler) >> 0) & 0xFFFF;
	mIDT[pIndex].segment = GDT::GetSelector(GDT_CODE64_SEL);
	mIDT[pIndex].ist = 0x0;
	mIDT[pIndex].type = pType;
	mIDT[pIndex].dpl = IDT_DPL0;
	mIDT[pIndex].present = IDT_PRESENT;
	mIDT[pIndex].offset_m = (reinterpret_cast<uint64_t>(pHandler) >> 16) & 0xFFFF;
	mIDT[pIndex].offset_h = (reinterpret_cast<uint64_t>(pHandler) >> 32) & 0xFFFFFFFF;
}

extern "C" void isr_handler(uint64_t pInt, CPUState_t *pState) {

	char _TempString[16];
	
	Interrupt::mInterruptCount[pInt] += 1;
	
	Console::Print("Interrupt ");
	Console::Print(utoa(pInt, _TempString, 10));
	Console::Print(" occured ");
	Console::Print(utoa(Interrupt::mInterruptCount[pInt], _TempString, 10));
	Console::Print(" times!\n");
	
}

extern "C" void exception_handler(uint64_t pInt, CPUState_t *pState) {
	
	char _TempString[32];
	uint64_t _cr2 = 0;
	
	asm volatile (
		"mov %%cr2, %0;\n"
		: "=r" (_cr2)
	);
	
	Console::SetTitleText("HeppOS - Exception Handler");
	Console::SetBGColor(0x000000AA);
	Console::SetFGColor(0x0000FFFF);
	
	Console::Clear();
	Console::Print("\n");
	Console::Print("ERROR - Exception 0x");
	Console::Print(utoa(pInt, _TempString, 16));
	Console::Print(" (");
	Console::Print(utoa(pInt, _TempString, 10));
	Console::Print(") occured!\n");
	Console::Print("\n");
	Console::Print("\n");
	
	
	Console::Print("Error code=0x");
	Console::Print(utoa(pState->error_code, _TempString, 16));
	Console::Print(" (");
	Console::Print(utoa(pState->error_code, _TempString, 10));
	Console::Print(")\n");
	Console::Print("\n");
	
	Console::Print("RFLAGS=0x");
	Console::Print(htoa(pState->rflags, _TempString));
	Console::Print("\n");
	Console::Print("\n");
	
	
	Console::Print("RAX=0x");
	Console::Print(htoa(pState->rax, _TempString));
	Console::Print(" \tRBX=0x");
	Console::Print(htoa(pState->rbx, _TempString));
	Console::Print("\n");
	
	Console::Print("RCX=0x");
	Console::Print(htoa(pState->rcx, _TempString));
	Console::Print(" \tRDX=0x");
	Console::Print(htoa(pState->rdx, _TempString));
	Console::Print("\n");
	
	Console::Print("RSI=0x");
	Console::Print(htoa(pState->rsi, _TempString));
	Console::Print(" \tRDI=0x");
	Console::Print(htoa(pState->rdi, _TempString));
	Console::Print("\n");
	
	Console::Print("RSP=0x");
	Console::Print(htoa(pState->rsp, _TempString));
	Console::Print(" \tRBP=0x");
	Console::Print(htoa(pState->rbp, _TempString));
	Console::Print("\n");
	Console::Print("\n");



	Console::Print("R8 =0x");
	Console::Print(htoa(pState->r8, _TempString));
	Console::Print(" \tR9 =0x");
	Console::Print(htoa(pState->r9, _TempString));
	Console::Print("\n");
	
	Console::Print("R10=0x");
	Console::Print(htoa(pState->r10, _TempString));
	Console::Print(" \tR11=0x");
	Console::Print(htoa(pState->r11, _TempString));
	Console::Print("\n");
	
	Console::Print("R12=0x");
	Console::Print(htoa(pState->r12, _TempString));
	Console::Print(" \tR13=0x");
	Console::Print(htoa(pState->r13, _TempString));
	Console::Print("\n");
	
	Console::Print("R14=0x");
	Console::Print(htoa(pState->r14, _TempString));
	Console::Print(" \tR15=0x");
	Console::Print(htoa(pState->r15, _TempString));
	Console::Print("\n");
	Console::Print("\n");
	
	
	Console::Print("RIP=0x");
	Console::Print(htoa(pState->rip, _TempString));
	Console::Print("\n");
	Console::Print("\n");
	
	Console::Print("CS =0x");
	Console::Print(htoa(pState->cs, _TempString));
	Console::Print("\n");
	Console::Print("SS =0x");
	Console::Print(htoa(pState->ss, _TempString));
	Console::Print("\n");
	Console::Print("\n");

	Console::Print("CR2=0x");
	Console::Print(htoa(_cr2, _TempString));
	Console::Print("\n");
	Console::Print("\n");
	
	
	// Loop forever because of Exception
	asm volatile ("cli;\n");
	for (;;) {
		asm volatile ("hlt;\n");
	}
}
