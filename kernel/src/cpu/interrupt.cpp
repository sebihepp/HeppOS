
#include <cpu/interrupt.h>

#include <video/console.h>
#include <cpu/gdt.h>
#include <cstub.h>
#include <cpu/io.h>
#include <cpu/msr.h>
#include <cpu/pic.h>


// For quick testing - needs to be put in a string.h or something similar
char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);
char *htoa(uint64_t num, char* str);


extern "C" void *gISRHandlerAddressTable[INTERRUPT_MAX_COUNT];

ISRHandler_t CInterrupt::mISRHandler[INTERRUPT_MAX_COUNT][INTERRUPT_MAX_HANDLER];

IDTEntry_t CInterrupt::mIDT[INTERRUPT_MAX_COUNT];
IDTD_t CInterrupt::mIDTD;

uint64_t CInterrupt::mInterruptCount[INTERRUPT_MAX_COUNT];

ReturnValue_t CInterrupt::Init(void) {

	ReturnValue_t _RetVal = RETVAL_OK;
	
	// Init PIC (Programmable Interrupt Controller)
	_RetVal = CPIC::Init(INTERRUPT_PIC_OFFSET);
	if (IS_ERROR(_RetVal))
		return _RetVal;
	
	// Clear IDT and IDTD
	memset(mIDT, 0, sizeof(mIDT));
	memset(&mIDTD, 0, sizeof(mIDTD));
	memset(mInterruptCount, 0, sizeof(mInterruptCount));
	
	// Clear ISRHandler Array
	for (uint64_t a = 0; a < INTERRUPT_MAX_COUNT; a++) {
		for (uint64_t b = 0; b < INTERRUPT_MAX_HANDLER; b++) {
			mISRHandler[a][b] = NULL;
		}		
	}
	
	// Exceptions
	for (uint64_t i = 0; i < 32; i++) {
		if (gISRHandlerAddressTable[i] != NULL) {
			SetIDTEntry(i, gISRHandlerAddressTable[i], IDT_TYPE_TRAP);
		}
	}
	
	// Interrupts
	for (uint64_t i = 32; i < 256; i++) {
		if (gISRHandlerAddressTable[i] != NULL) {
			SetIDTEntry(i, gISRHandlerAddressTable[i], IDT_TYPE_GATE);
		}
	}
	
	
	// IDTD
	mIDTD.size = sizeof(mIDT) - 1;
	mIDTD.offset = reinterpret_cast<uint64_t>(&mIDT);
	
	LoadIDT();
	
	EnableInterrupts();
	
	//Unmask all
	CPIC::UnmaskAll();
	
	return RETVAL_OK;
}

void CInterrupt::LoadIDT(void) {
	
	asm volatile (
		"lidt %0;\n"
		:
		: "m" (mIDTD)
		:
	);
}

void CInterrupt::RegisterHandler(uint8_t pIndex, ISRHandler_t pHandler) {
	for (uint64_t i = 0; i < INTERRUPT_MAX_HANDLER; i++) {
		if (mISRHandler[pIndex][i] == pHandler)
			return;
	}
	for (uint64_t i = 0; i < INTERRUPT_MAX_HANDLER; i++) {
		if (mISRHandler[pIndex][i] == NULL) {
			mISRHandler[pIndex][i] = pHandler;
			break;
		}
	}
}

void CInterrupt::UnregisterHandler(uint8_t pIndex, ISRHandler_t pHandler) {
	for (uint64_t i = 0; i < INTERRUPT_MAX_HANDLER; i++) {
		if (mISRHandler[pIndex][i] == pHandler) {
			mISRHandler[pIndex][i] = NULL;
		}
	}
}

void CInterrupt::SetIDTEntry(uint8_t pIndex, void *pHandler, uint8_t pType) {
	mIDT[pIndex].offset_l = (reinterpret_cast<uint64_t>(pHandler) >> 0) & 0xFFFF;
	mIDT[pIndex].segment = CGDT::GetSelector(GDT_CODE64_SEL);
	mIDT[pIndex].ist = 0x0;
	mIDT[pIndex].type = pType;
	mIDT[pIndex].dpl = IDT_DPL0;
	mIDT[pIndex].present = IDT_PRESENT;
	mIDT[pIndex].offset_m = (reinterpret_cast<uint64_t>(pHandler) >> 16) & 0xFFFF;
	mIDT[pIndex].offset_h = (reinterpret_cast<uint64_t>(pHandler) >> 32) & 0xFFFFFFFF;
}

uint64_t CInterrupt::GetInterruptCount(uint8_t pInt) {
	return mInterruptCount[pInt];
}

extern "C" void ISRHandler(uint64_t pInt, CPUState_t *pState) {

	//char _TempString[16];
	
/* 	CConsole::Print("Interrupt ");
	CConsole::Print(utoa(pInt, _TempString, 10));
	CConsole::Print(" occured ");
	CConsole::Print(utoa(Interrupt::mInterruptCount[pInt], _TempString, 10));
	CConsole::Print(" times!\n"); */
	
	CInterrupt::mInterruptCount[pInt] += 1;
	
	//Check for spurious interrupt and return if yes
	if (CPIC::CheckSpurious(pInt)) {
		CPIC::SendEOI(pInt);
		return;
	}
	
	for (uint64_t i = 0; i < INTERRUPT_MAX_HANDLER; i++) {
		if (CInterrupt::mISRHandler[pInt][i] != NULL)
			CInterrupt::mISRHandler[pInt][i](pInt, pState);
	}
	
	//Send EOI (CPIC will make sure it is only send if pInt is a PIC IRQ.
	CPIC::SendEOI(pInt);
}

extern "C" void ExceptionHandler(uint64_t pInt, CPUState_t *pState) {
	
	char _TempString[32];
	uint64_t _CR0 = 0;
	uint64_t _CR2 = 0;
	uint64_t _CR3 = 0;
	uint64_t _CR4 = 0;
	uint64_t _CR8 = 0;
	uint64_t _DR6 = 0;
	uint64_t _DR7 = 0;
	uint64_t _EFER = 0;
	
	asm volatile (
		"mov %%cr0, %0;\n"
		"mov %%cr2, %1;\n"
		"mov %%cr3, %2;\n"
		"mov %%cr4, %3;\n"
		"mov %%cr8, %4;\n"
		"mov %%dr6, %5;\n"
		"mov %%dr7, %6;\n"
		: "=r" (_CR0), "=r" (_CR2), "=r" (_CR3), "=r" (_CR4), "=r" (_CR8), "=r" (_DR6), "=r" (_DR7)
	);
	
	ReadMSR(MSR_EFER, &_EFER);
	
	CInterrupt::mInterruptCount[pInt] += 1;
	
	CConsole::SetTitleText("HeppOS - Exception Handler");
	CConsole::SetBGColor(0x000000AA);
	CConsole::SetFGColor(0x0000FFFF);
	
	CConsole::Clear();
	CConsole::Print("\n");
	CConsole::Print("ERROR - Exception 0x");
	CConsole::Print(utoa(pInt, _TempString, 16));
	CConsole::Print(" (");
	CConsole::Print(utoa(pInt, _TempString, 10));
	CConsole::Print(") occured! (");

	switch (pInt) {
		case 0x00:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x09:
		case 0x0A:
		case 0x0B:
		case 0x0C:
		case 0x0D:
		case 0x0E:
		case 0x10:
		case 0x11:
		case 0x13:
		case 0x14:
		case 0x15:
		case 0x1C:
		case 0x1D:
			CConsole::Print("Fault");
			break;
		case 0x03:
		case 0x04:
			CConsole::Print("Trap");
			break;
		case 0x01:
			CConsole::Print("Fault/Trap");
			break;
		case 0x02:
			CConsole::Print("NMI");
			break;
		case 0x08:
		case 0x12:
			CConsole::Print("Abort");
			break;
		case 0x0F:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:
		case 0x1A:
		case 0x1B:
		case 0x1F:
			CConsole::Print("Reserved");
			break;


		default:
			CConsole::Print("Unknown");
	}
	
	CConsole::Print(")\n");
	CConsole::Print("\n");
	CConsole::Print("\n");
	
	CInterrupt::PrintErrorCode(pInt, pState->error_code);
	
	CConsole::Print("RFLAGS=0x");
	CConsole::Print(htoa(pState->rflags, _TempString));
	CConsole::Print("\n");
	CConsole::Print("\n");
	
	
	CConsole::Print("RAX=0x");
	CConsole::Print(htoa(pState->rax, _TempString));
	CConsole::Print(" \tRBX=0x");
	CConsole::Print(htoa(pState->rbx, _TempString));
	CConsole::Print("\n");
	
	CConsole::Print("RCX=0x");
	CConsole::Print(htoa(pState->rcx, _TempString));
	CConsole::Print(" \tRDX=0x");
	CConsole::Print(htoa(pState->rdx, _TempString));
	CConsole::Print("\n");
	
	CConsole::Print("RSI=0x");
	CConsole::Print(htoa(pState->rsi, _TempString));
	CConsole::Print(" \tRDI=0x");
	CConsole::Print(htoa(pState->rdi, _TempString));
	CConsole::Print("\n");
	
	CConsole::Print("RSP=0x");
	CConsole::Print(htoa(pState->rsp, _TempString));
	CConsole::Print(" \tRBP=0x");
	CConsole::Print(htoa(pState->rbp, _TempString));
	CConsole::Print("\n");
	CConsole::Print("\n");



	CConsole::Print("R8 =0x");
	CConsole::Print(htoa(pState->r8, _TempString));
	CConsole::Print(" \tR9 =0x");
	CConsole::Print(htoa(pState->r9, _TempString));
	CConsole::Print("\n");
	
	CConsole::Print("R10=0x");
	CConsole::Print(htoa(pState->r10, _TempString));
	CConsole::Print(" \tR11=0x");
	CConsole::Print(htoa(pState->r11, _TempString));
	CConsole::Print("\n");
	
	CConsole::Print("R12=0x");
	CConsole::Print(htoa(pState->r12, _TempString));
	CConsole::Print(" \tR13=0x");
	CConsole::Print(htoa(pState->r13, _TempString));
	CConsole::Print("\n");
	
	CConsole::Print("R14=0x");
	CConsole::Print(htoa(pState->r14, _TempString));
	CConsole::Print(" \tR15=0x");
	CConsole::Print(htoa(pState->r15, _TempString));
	CConsole::Print("\n");
	CConsole::Print("\n");
	
	
	CConsole::Print("RIP=0x");
	CConsole::Print(htoa(pState->rip, _TempString));
	CConsole::Print("\n");
	CConsole::Print("\n");
	
	CConsole::Print("CS =0x");
	CConsole::Print(htoa(pState->cs, _TempString));
	CConsole::Print("\n");
	CConsole::Print("SS =0x");
	CConsole::Print(htoa(pState->ss, _TempString));
	CConsole::Print("\n");
	CConsole::Print("DS =0x");
	CConsole::Print(htoa(pState->ds, _TempString));
	CConsole::Print("\n");
	CConsole::Print("ES =0x");
	CConsole::Print(htoa(pState->es, _TempString));
	CConsole::Print("\n");
	CConsole::Print("FS =0x");
	CConsole::Print(htoa(pState->fs, _TempString));
	CConsole::Print("\n");
	CConsole::Print("GS =0x");
	CConsole::Print(htoa(pState->gs, _TempString));
	CConsole::Print("\n");
	CConsole::Print("\n");


	CConsole::Print("CR0=0x");
	CConsole::Print(htoa(_CR0, _TempString));
	CConsole::Print(" \tCR2=0x");
	CConsole::Print(htoa(_CR2, _TempString));
	CConsole::Print("\n");

	CConsole::Print("CR3=0x");
	CConsole::Print(htoa(_CR3, _TempString));
	CConsole::Print(" \tCR4=0x");
	CConsole::Print(htoa(_CR4, _TempString));
	CConsole::Print("\n");
	CConsole::Print("\n");

	CConsole::Print("CR8=0x");
	CConsole::Print(htoa(_CR8, _TempString));
	CConsole::Print("\n");
	CConsole::Print("\n");
	
	CConsole::Print("EFER=0x");
	CConsole::Print(htoa(_EFER, _TempString));
	CConsole::Print("\n");
	CConsole::Print("\n");
	
	CConsole::Print("DR6=0x");
	CConsole::Print(htoa(_DR6, _TempString));
	CConsole::Print(" \tDR7=0x");
	CConsole::Print(htoa(_DR7, _TempString));
	CConsole::Print("\n");
	CConsole::Print("\n");
	
	// Loop forever because of Exception
	asm volatile ("cli;\n");
	for (;;) {
		asm volatile ("hlt;\n");
	}
}

void CInterrupt::PrintErrorCode(uint64_t pInt, uint64_t pErrorCode) {
	
	char _TempString[32];
	
	CConsole::Print("Error code=0x");
	CConsole::Print(utoa(pErrorCode, _TempString, 16));
	CConsole::Print(" (");
	CConsole::Print(utoa(pErrorCode, _TempString, 10));
	CConsole::Print(")\n");
	
	CConsole::Print("Meaning:\n");
	switch (pInt) {
		
		case 0x0e:
			if (pErrorCode & 0x01) {
				CConsole::Print(" Protection fault\n");
			} else {
				CConsole::Print(" Page not present\n");
			}
			if (pErrorCode & 0x02) {
				CConsole::Print(" Write access\n");
			} else {
				CConsole::Print(" Read access\n");
			}
			if (pErrorCode & 0x04) {
				CConsole::Print(" User access\n");
			} else {
				CConsole::Print(" Superuser access\n");
			}
			if (pErrorCode & 0x08) {
				CConsole::Print(" Reserved bit set\n");
			}
			if (pErrorCode & 0x10) {
				CConsole::Print(" Instruction fetch\n");
			} else {
				CConsole::Print(" Data access\n");
			}
			if (pErrorCode & 0x20) {
				CConsole::Print(" Protection key violation\n");
			}
			if (pErrorCode & 0x40) {
				CConsole::Print(" Shadow-stack access fault\n");
			}
			if (pErrorCode & 0x8000) {
				CConsole::Print(" SGX violation\n");
			}
			
			break;
			
		default:
			CConsole::Print("---\n");
	}
	CConsole::Print("\n");
	
}

