
#include <cpu/interrupt.h>

#include <cpu/gdt.h>
#include <cpu/io.h>
#include <cpu/msr.h>
#include <cpu/pic.h>

#include <log.h>
#include <kstring.h>



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
			SetIDTEntry(i, gISRHandlerAddressTable[i], IDT_TYPE_GATE);
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
	
	CLog::Print("\n");
	CLog::Print("ERROR - Exception 0x");
	CLog::Print(utoa(pInt, _TempString, 16));
	CLog::Print(" (");
	CLog::Print(utoa(pInt, _TempString, 10));
	CLog::Print(") occured! (");

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
			CLog::Print("Fault");
			break;
		case 0x03:
		case 0x04:
			CLog::Print("Trap");
			break;
		case 0x01:
			CLog::Print("Fault/Trap");
			break;
		case 0x02:
			CLog::Print("NMI");
			break;
		case 0x08:
		case 0x12:
			CLog::Print("Abort");
			break;
		case 0x0F:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:
		case 0x1A:
		case 0x1B:
		case 0x1F:
			CLog::Print("Reserved");
			break;


		default:
			CLog::Print("Unknown");
	}
	
	CLog::Print(")\n");
	CLog::Print("\n");
	CLog::Print("\n");
	
	CInterrupt::PrintErrorCode(pInt, pState->error_code);
	
	CLog::Print("RFLAGS=0x");
	CLog::Print(utoa(pState->rflags, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("\n");
	
	
	CLog::Print("RAX=0x");
	CLog::Print(utoa(pState->rax, _TempString, 16));
	CLog::Print(" \tRBX=0x");
	CLog::Print(utoa(pState->rbx, _TempString, 16));
	CLog::Print("\n");
	
	CLog::Print("RCX=0x");
	CLog::Print(utoa(pState->rcx, _TempString, 16));
	CLog::Print(" \tRDX=0x");
	CLog::Print(utoa(pState->rdx, _TempString, 16));
	CLog::Print("\n");
	
	CLog::Print("RSI=0x");
	CLog::Print(utoa(pState->rsi, _TempString, 16));
	CLog::Print(" \tRDI=0x");
	CLog::Print(utoa(pState->rdi, _TempString, 16));
	CLog::Print("\n");
	
	CLog::Print("RSP=0x");
	CLog::Print(utoa(pState->rsp, _TempString, 16));
	CLog::Print(" \tRBP=0x");
	CLog::Print(utoa(pState->rbp, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("\n");



	CLog::Print("R8 =0x");
	CLog::Print(utoa(pState->r8, _TempString, 16));
	CLog::Print(" \tR9 =0x");
	CLog::Print(utoa(pState->r9, _TempString, 16));
	CLog::Print("\n");
	
	CLog::Print("R10=0x");
	CLog::Print(utoa(pState->r10, _TempString, 16));
	CLog::Print(" \tR11=0x");
	CLog::Print(utoa(pState->r11, _TempString, 16));
	CLog::Print("\n");
	
	CLog::Print("R12=0x");
	CLog::Print(utoa(pState->r12, _TempString, 16));
	CLog::Print(" \tR13=0x");
	CLog::Print(utoa(pState->r13, _TempString, 16));
	CLog::Print("\n");
	
	CLog::Print("R14=0x");
	CLog::Print(utoa(pState->r14, _TempString, 16));
	CLog::Print(" \tR15=0x");
	CLog::Print(utoa(pState->r15, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("\n");
	
	
	CLog::Print("RIP=0x");
	CLog::Print(utoa(pState->rip, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("\n");
	
	CLog::Print("CS =0x");
	CLog::Print(utoa(pState->cs, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("SS =0x");
	CLog::Print(utoa(pState->ss, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("DS =0x");
	CLog::Print(utoa(pState->ds, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("ES =0x");
	CLog::Print(utoa(pState->es, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("FS =0x");
	CLog::Print(utoa(pState->fs, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("GS =0x");
	CLog::Print(utoa(pState->gs, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("\n");


	CLog::Print("CR0=0x");
	CLog::Print(utoa(_CR0, _TempString, 16));
	CLog::Print(" \tCR2=0x");
	CLog::Print(utoa(_CR2, _TempString, 16));
	CLog::Print("\n");

	CLog::Print("CR3=0x");
	CLog::Print(utoa(_CR3, _TempString, 16));
	CLog::Print(" \tCR4=0x");
	CLog::Print(utoa(_CR4, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("\n");

	CLog::Print("CR8=0x");
	CLog::Print(utoa(_CR8, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("\n");
	
	CLog::Print("EFER=0x");
	CLog::Print(utoa(_EFER, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("\n");
	
	CLog::Print("DR6=0x");
	CLog::Print(utoa(_DR6, _TempString, 16));
	CLog::Print(" \tDR7=0x");
	CLog::Print(utoa(_DR7, _TempString, 16));
	CLog::Print("\n");
	CLog::Print("\n");
	
	// Loop forever because of Exception
	asm volatile ("cli;\n");
	for (;;) {
		asm volatile ("hlt;\n");
	}
}

void CInterrupt::PrintErrorCode(uint64_t pInt, uint64_t pErrorCode) {
	
	char _TempString[32];
	
	CLog::Print("Error code=0x");
	CLog::Print(utoa(pErrorCode, _TempString, 16));
	CLog::Print(" (");
	CLog::Print(utoa(pErrorCode, _TempString, 10));
	CLog::Print(")\n");
	
	CLog::Print("Meaning:\n");
	switch (pInt) {
		
		case 0x0e:
			if (pErrorCode & 0x01) {
				CLog::Print(" Protection fault\n");
			} else {
				CLog::Print(" Page not present\n");
			}
			if (pErrorCode & 0x02) {
				CLog::Print(" Write access\n");
			} else {
				CLog::Print(" Read access\n");
			}
			if (pErrorCode & 0x04) {
				CLog::Print(" User access\n");
			} else {
				CLog::Print(" Superuser access\n");
			}
			if (pErrorCode & 0x08) {
				CLog::Print(" Reserved bit set\n");
			}
			if (pErrorCode & 0x10) {
				CLog::Print(" Instruction fetch\n");
			} else {
				CLog::Print(" Data access\n");
			}
			if (pErrorCode & 0x20) {
				CLog::Print(" Protection key violation\n");
			}
			if (pErrorCode & 0x40) {
				CLog::Print(" Shadow-stack access fault\n");
			}
			if (pErrorCode & 0x8000) {
				CLog::Print(" SGX violation\n");
			}
			
			break;
			
		default:
			CLog::Print("---\n");
	}
	CLog::Print("\n");
	
}

