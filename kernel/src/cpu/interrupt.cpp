
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

ISRHandler_t Interrupt::mISRHandler[INTERRUPT_MAX_COUNT][INTERRUPT_MAX_HANDLER];

IDTEntry_t Interrupt::mIDT[INTERRUPT_MAX_COUNT];
IDTD_t Interrupt::mIDTD;

uint64_t Interrupt::mInterruptCount[INTERRUPT_MAX_COUNT];

ReturnValue_t Interrupt::Init(void) {

	ReturnValue_t _RetVal = RETVAL_OK;
	
	// Init PIC (Programmable Interrupt Controller)
	_RetVal = PIC::Init(INTERRUPT_PIC_OFFSET);
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

void Interrupt::UnregisterHandler(uint8_t pIndex, ISRHandler_t pHandler) {
	for (uint64_t i = 0; i < INTERRUPT_MAX_HANDLER; i++) {
		if (mISRHandler[pIndex][i] == pHandler) {
			mISRHandler[pIndex][i] = NULL;
		}
	}
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

uint64_t Interrupt::GetInterruptCount(uint8_t pInt) {
	return mInterruptCount[pInt];
}

extern "C" void ISRHandler(uint64_t pInt, CPUState_t *pState) {

	//char _TempString[16];
	
	Interrupt::mInterruptCount[pInt] += 1;
	
/* 	Console::Print("Interrupt ");
	Console::Print(utoa(pInt, _TempString, 10));
	Console::Print(" occured ");
	Console::Print(utoa(Interrupt::mInterruptCount[pInt], _TempString, 10));
	Console::Print(" times!\n"); */
	
	for (uint64_t i = 0; i < INTERRUPT_MAX_HANDLER; i++) {
		if (Interrupt::mISRHandler[pInt][i] != NULL)
			Interrupt::mISRHandler[pInt][i](pInt, pState);
	}
	
	if ((pInt >= PIC::GetOffset()) && (pInt < (PIC::GetOffset() + PIC::GetIntLineCount()))) {
		//Console::Print("EOI sent!\n");
		PIC::SendEOI(pInt - PIC::GetOffset());
	}

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
	
	Interrupt::mInterruptCount[pInt] += 1;
	
	Console::SetTitleText("HeppOS - Exception Handler");
	Console::SetBGColor(0x000000AA);
	Console::SetFGColor(0x0000FFFF);
	
	Console::Clear();
	Console::Print("\n");
	Console::Print("ERROR - Exception 0x");
	Console::Print(utoa(pInt, _TempString, 16));
	Console::Print(" (");
	Console::Print(utoa(pInt, _TempString, 10));
	Console::Print(") occured! (");
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
			Console::Print("Fault");
			break;
		case 0x03:
		case 0x04:
			Console::Print("Trap");
			break;
		case 0x01:
			Console::Print("Fault/Trap");
			break;
		case 0x02:
			Console::Print("NMI");
			break;
		case 0x08:
		case 0x12:
			Console::Print("Abort");
			break;
		case 0x0F:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:
		case 0x1A:
		case 0x1B:
		case 0x1F:
			Console::Print("Reserved");
			break;


		default:
			Console::Print("Unknown");
	}
	
	Console::Print(")\n");
	Console::Print("\n");
	Console::Print("\n");
	
	Interrupt::PrintErrorCode(pInt, pState->error_code);
	
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


	Console::Print("CR0=0x");
	Console::Print(htoa(_CR0, _TempString));
	Console::Print(" \tCR2=0x");
	Console::Print(htoa(_CR2, _TempString));
	Console::Print("\n");

	Console::Print("CR3=0x");
	Console::Print(htoa(_CR3, _TempString));
	Console::Print(" \tCR4=0x");
	Console::Print(htoa(_CR4, _TempString));
	Console::Print("\n");
	Console::Print("\n");

	Console::Print("CR8=0x");
	Console::Print(htoa(_CR8, _TempString));
	Console::Print("\n");
	Console::Print("\n");
	
	Console::Print("EFER=0x");
	Console::Print(htoa(_EFER, _TempString));
	Console::Print("\n");
	Console::Print("\n");
	
	Console::Print("DR6=0x");
	Console::Print(htoa(_DR6, _TempString));
	Console::Print(" \tDR7=0x");
	Console::Print(htoa(_DR7, _TempString));
	Console::Print("\n");
	Console::Print("\n");
	
	// Loop forever because of Exception
	asm volatile ("cli;\n");
	for (;;) {
		asm volatile ("hlt;\n");
	}
}

void Interrupt::PrintErrorCode(uint64_t pInt, uint64_t pErrorCode) {
	
	char _TempString[32];
	
	Console::Print("Error code=0x");
	Console::Print(utoa(pErrorCode, _TempString, 16));
	Console::Print(" (");
	Console::Print(utoa(pErrorCode, _TempString, 10));
	Console::Print(")\n");
	
	Console::Print("Meaning:\n");
	switch (pInt) {
		
		case 0x0e:
			if (pErrorCode & 0x01) {
				Console::Print(" Protection fault\n");
			} else {
				Console::Print(" Page not present\n");
			}
			if (pErrorCode & 0x02) {
				Console::Print(" Write access\n");
			} else {
				Console::Print(" Read access\n");
			}
			if (pErrorCode & 0x04) {
				Console::Print(" User access\n");
			} else {
				Console::Print(" Superuser access\n");
			}
			if (pErrorCode & 0x08) {
				Console::Print(" Reserved bit set\n");
			}
			if (pErrorCode & 0x10) {
				Console::Print(" Instruction fetch\n");
			} else {
				Console::Print(" Data access\n");
			}
			if (pErrorCode & 0x20) {
				Console::Print(" Protection key violation\n");
			}
			if (pErrorCode & 0x40) {
				Console::Print(" Shadow-stack access fault\n");
			}
			if (pErrorCode & 0x8000) {
				Console::Print(" SGX violation\n");
			}
			
			break;
			
		default:
			Console::Print("---\n");
	}
	Console::Print("\n");
	
}
