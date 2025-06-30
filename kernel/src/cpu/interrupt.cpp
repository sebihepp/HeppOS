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
			RegisterHandler(i, ExceptionHandler);
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

void ExceptionHandler(uint64_t pInt, CPUState_t *pState) {
	
	uint64_t _CR0 = 0;
	uint64_t _CR2 = 0;
	uint64_t _CR3 = 0;
	uint64_t _CR4 = 0;
	uint64_t _CR8 = 0;
	uint64_t _DR0 = 0;
	uint64_t _DR1 = 0;
	uint64_t _DR2 = 0;
	uint64_t _DR3 = 0;
	uint64_t _DR6 = 0;
	uint64_t _DR7 = 0;
	uint64_t _EFER = 0;
	
	asm volatile (
		"mov %%cr0, %0;\n"
		"mov %%cr2, %1;\n"
		"mov %%cr3, %2;\n"
		"mov %%cr4, %3;\n"
		"mov %%cr8, %4;\n"
		"mov %%dr0, %5;\n"
		"mov %%dr1, %6;\n"
		"mov %%dr2, %7;\n"
		"mov %%dr3, %8;\n"
		"mov %%dr6, %9;\n"
		"mov %%dr7, %10;\n"
		: "=r" (_CR0), "=r" (_CR2), "=r" (_CR3), "=r" (_CR4), "=r" (_CR8),
			"=r" (_DR0), "=r" (_DR1), "=r" (_DR2), "=r" (_DR3), "=r" (_DR6), "=r" (_DR7)
	);
	
	ReadMSR(MSR_EFER, &_EFER);
	
	CInterrupt::mInterruptCount[pInt] += 1;
	
	CLog::Print("\n");
	CLog::PrintF("ERROR - Exception 0x%02X (%d) - %s!\n", pInt, pInt, GetExceptionName(pInt));
	CLog::Print("\n");
	CLog::Print("\n");
	
	CInterrupt::PrintErrorCode(pInt, pState->error_code);
	
	CLog::PrintF("RFLAGS=0x%016lX\n", pState->rflags);
	CLog::Print("\n");
	
	
	CLog::PrintF("RAX=0x%016lX \tRBX=0x%016lX\n", pState->rax, pState->rbx);
	CLog::PrintF("RCX=0x%016lX \tRDX=0x%016lX\n", pState->rcx, pState->rdx);
	CLog::PrintF("RSI=0x%016lX \tRDI=0x%016lX\n", pState->rsi, pState->rdi);
	CLog::PrintF("RSP=0x%016lX \tRBP=0x%016lX\n", pState->rsp, pState->rbp);
	CLog::Print("\n");
	
	CLog::PrintF("R8 =0x%016lX \tR9 =0x%016lX\n", pState->r8, pState->r9);
	CLog::PrintF("R10=0x%016lX \tR11=0x%016lX\n", pState->r10, pState->r11);
	CLog::PrintF("R12=0x%016lX \tR13=0x%016lX\n", pState->r12, pState->r13);
	CLog::PrintF("R14=0x%016lX \tR15=0x%016lX\n", pState->r14, pState->r15);
	CLog::Print("\n");
	
	
	CLog::PrintF("RIP=0x%016lX\n", pState->rip);
	CLog::Print("\n");
	
	CLog::PrintF("CS=0x%04X \tSS=0x%04X\n", pState->cs, pState->ss);
	CLog::PrintF("DS=0x%04X \tES=0x%04X\n", pState->ds, pState->es);
	CLog::PrintF("FS=0x%04X \tGS=0x%04X\n", pState->fs, pState->gs);
	CLog::Print("\n");

	CLog::PrintF("CR0=0x%016lX \tCR2=0x%016lX\n", _CR0, _CR2);
	CLog::PrintF("CR3=0x%016lX \tCR4=0x%016lX\n", _CR3, _CR4);
	CLog::Print("\n");
	CLog::PrintF("CR8=0x%016lX\n", _CR8);
	CLog::Print("\n");
	
	CLog::PrintF("EFER=0x%016lX\n", _EFER);	
	CLog::Print("\n");
	
	CLog::PrintF("DR0=0x%016lX \tDR1=0x%016lX\n", _DR0, _DR1);
	CLog::PrintF("DR2=0x%016lX \tDR3=0x%016lX\n", _DR2, _DR3);
	CLog::PrintF("DR6=0x%016lX \tDR7=0x%016lX\n", _DR6, _DR7);
	CLog::Print("\n");
	
	// Loop forever because of Exception
	asm volatile ("cli;\n");
	for (;;) {
		asm volatile ("hlt;\n");
	}
}

void CInterrupt::PrintErrorCode(uint64_t pInt, uint64_t pErrorCode) {
	
	CLog::PrintF("Error code=0x%lX (%ld)\n", pErrorCode, pErrorCode);
	
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

const char *GetExceptionName(uint64_t pInt) {

	switch (pInt) {
		case 0x00:
			return "Division Error (#DE)";
			break;
		case 0x01:
			return "Debug (#DB)";
			break;
		case 0x02:
			return "Non-maskable Interrupt (#NMI)";
			break;
		case 0x03:
			return "Breakpoint (#BP)";
			break;
		case 0x04:
			return "Overflow (#OF)";
			break;
		case 0x05:
			return "Bound range exceeded (#BR)";
			break;
		case 0x06:
			return "Invalid Opcode (#UD)";
			break;
		case 0x07:
			return "Device not available (#NM)";
			break;
		case 0x08:
			return "Double Fault (#DF)";
			break;
		case 0x09:
			return "Coprocessor Segment Overrun";
			break;
		case 0x0A:
			return "Invalid TSS (#TS)";
			break;
		case 0x0B:
			return "Segment not present (#NP)";
			break;
		case 0x0C:
			return "Stack-Segment Fault (#SS)";
			break;
		case 0x0D:
			return "General Protection Fault (#GP)";
			break;
		case 0x0E:
			return "Page Fault (#PF)";
			break;
		case 0x0F:
			return "Reserved";
			break;
		case 0x10:
			return "x87 Floating-Point Exception (#MF)";
			break;
		case 0x11:
			return "Alignment Check (#AC)";
			break;
		case 0x12:
			return "Machine Check (#MC)";
			break;
		case 0x13:
			return "SIMD Floating-Point Exception (#XF)";
			break;
		case 0x14:
			return "Virtualization Exception (#VE)";
			break;
		case 0x15:
			return "Control Protection Exception (#CP)";
			break;
		case 0x1C:
			return "Hypervisor Injection Exception (#HV)";
			break;
		case 0x1D:
			return "VMM Communication Exception (#VC)";
			break;
		case 0x1E:
			return "Security Exception (#SX)";
			break;
		case 0x1F:
			return "Reserved";
			break;


		default:
			return "Unknown";
			break;
	}
	return "";
}
