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

#include <boot_log.h>
#include <kstring.h>



extern "C" void *isr_handler_address_table[INTERRUPT_MAX_COUNT];

ISRHandler Interrupt::isr_handler_[INTERRUPT_MAX_COUNT][INTERRUPT_MAX_HANDLER];

IDTEntry Interrupt::idt_[INTERRUPT_MAX_COUNT];
IDTD Interrupt::idtd_;

uint64_t Interrupt::interrupt_count_[INTERRUPT_MAX_COUNT];

ReturnValue Interrupt::Init(void) {

	ReturnValue ret_val = ReturnValueOk;
	
	// Init PIC (Programmable Interrupt Controller)
	ret_val = PIC::Init(INTERRUPT_PIC_OFFSET);
	if (IS_ERROR(ret_val))
		return ret_val;
	
	// Clear IDT and IDTD
	memset(idt_, 0, sizeof(idt_));
	memset(&idtd_, 0, sizeof(idtd_));
	memset(interrupt_count_, 0, sizeof(interrupt_count_));
	
	// Clear ISRHandler Array
	for (uint64_t a = 0; a < INTERRUPT_MAX_COUNT; a++) {
		for (uint64_t b = 0; b < INTERRUPT_MAX_HANDLER; b++) {
			isr_handler_[a][b] = NULL;
		}		
	}
	
	// Exceptions
	for (uint64_t i = 0; i < 32; i++) {
		if (isr_handler_address_table[i] != NULL) {
			SetIDTEntry(i, isr_handler_address_table[i], IDT_TYPE_GATE);
			RegisterHandler(i, ExceptionHandler);
		}
	}
	
	// Interrupts
	for (uint64_t i = 32; i < 256; i++) {
		if (isr_handler_address_table[i] != NULL) {
			SetIDTEntry(i, isr_handler_address_table[i], IDT_TYPE_GATE);
		}
	}
	
	
	// IDTD
	idtd_.size = sizeof(idt_) - 1;
	idtd_.offset = reinterpret_cast<uint64_t>(&idt_);
	
	LoadIDT();
	
	EnableInterrupts();
	
	//Unmask all
	PIC::UnmaskAll();
	
	return ReturnValueOk;
}

void Interrupt::LoadIDT(void) {
	
	asm volatile (
		"lidt %0;\n"
		:
		: "m" (idtd_)
		:
	);
}

void Interrupt::RegisterHandler(uint8_t index, ISRHandler handler) {
	for (uint64_t i = 0; i < INTERRUPT_MAX_HANDLER; i++) {
		if (isr_handler_[index][i] == handler)
			return;
	}
	for (uint64_t i = 0; i < INTERRUPT_MAX_HANDLER; i++) {
		if (isr_handler_[index][i] == NULL) {
			isr_handler_[index][i] = handler;
			break;
		}
	}
}

void Interrupt::UnregisterHandler(uint8_t index, ISRHandler handler) {
	for (uint64_t i = 0; i < INTERRUPT_MAX_HANDLER; i++) {
		if (isr_handler_[index][i] == handler) {
			isr_handler_[index][i] = NULL;
		}
	}
}

void Interrupt::SetIDTEntry(uint8_t index, void *handler, uint8_t type) {
	idt_[index].offset_l = (reinterpret_cast<uint64_t>(handler) >> 0) & 0xFFFF;
	idt_[index].segment = GDT::GetSelector(GDT_CODE64_SEL);
	idt_[index].ist = 0x0;
	idt_[index].type = type;
	idt_[index].dpl = IDT_DPL0;
	idt_[index].present = IDT_PRESENT;
	idt_[index].offset_m = (reinterpret_cast<uint64_t>(handler) >> 16) & 0xFFFF;
	idt_[index].offset_h = (reinterpret_cast<uint64_t>(handler) >> 32) & 0xFFFFFFFF;
}

uint64_t Interrupt::GetInterruptCount(uint8_t index) {
	return interrupt_count_[index];
}

extern "C" void StdISRHandler(uint64_t index, CPUState *state) {

	Interrupt::interrupt_count_[index] += 1;
	
	//Check for spurious interrupt and return if yes
	if (PIC::CheckSpurious(index)) {
		PIC::SendEOI(index);
		return;
	}
	
	for (uint64_t i = 0; i < INTERRUPT_MAX_HANDLER; i++) {
		if (Interrupt::isr_handler_[index][i] != NULL)
			Interrupt::isr_handler_[index][i](index, state);
	}
	
	//Send EOI (PIC will make sure it is only send if int is a PIC IRQ.
	PIC::SendEOI(index);
}

void ExceptionHandler(uint64_t index, CPUState *state) {
	
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
	
	Interrupt::interrupt_count_[index] += 1;
	
	BootLog::Print("\n");
	BootLog::PrintF("ERROR - Exception 0x%02x (%d) - %s!\n", index, index, GetExceptionName(index));
	BootLog::Print("\n");
	BootLog::Print("\n");
	
	Interrupt::PrintErrorCode(index, state->error_code);
	
	BootLog::PrintF("RFLAGS=0x%016lx\n", state->rflags);
	BootLog::Print("\n");
	
	
	BootLog::PrintF("RAX=0x%016lx \tRBX=0x%016lx\n", state->rax, state->rbx);
	BootLog::PrintF("RCX=0x%016lx \tRDX=0x%016lx\n", state->rcx, state->rdx);
	BootLog::PrintF("RSI=0x%016lx \tRDI=0x%016lx\n", state->rsi, state->rdi);
	BootLog::PrintF("RSP=0x%016lx \tRBP=0x%016lx\n", state->rsp, state->rbp);
	BootLog::Print("\n");
	
	BootLog::PrintF("R8 =0x%016lx \tR9 =0x%016lx\n", state->r8, state->r9);
	BootLog::PrintF("R10=0x%016lx \tR11=0x%016lx\n", state->r10, state->r11);
	BootLog::PrintF("R12=0x%016lx \tR13=0x%016lx\n", state->r12, state->r13);
	BootLog::PrintF("R14=0x%016lx \tR15=0x%016lx\n", state->r14, state->r15);
	BootLog::Print("\n");
	
	
	BootLog::PrintF("RIP=0x%016lx\n", state->rip);
	BootLog::Print("\n");
	
	BootLog::PrintF("CS=0x%04x \tSS=0x%04x\n", state->cs, state->ss);
	BootLog::PrintF("DS=0x%04x \tES=0x%04x\n", state->ds, state->es);
	BootLog::PrintF("FS=0x%04x \tGS=0x%04x\n", state->fs, state->gs);
	BootLog::Print("\n");

	BootLog::PrintF("CR0=0x%016lx \tCR2=0x%016lx\n", _CR0, _CR2);
	BootLog::PrintF("CR3=0x%016lx \tCR4=0x%016lx\n", _CR3, _CR4);
	BootLog::Print("\n");
	BootLog::PrintF("CR8=0x%016lx\n", _CR8);
	BootLog::Print("\n");
	
	BootLog::PrintF("EFER=0x%016lx\n", _EFER);	
	BootLog::Print("\n");
	
	BootLog::PrintF("DR0=0x%016lx \tDR1=0x%016lx\n", _DR0, _DR1);
	BootLog::PrintF("DR2=0x%016lx \tDR3=0x%016lx\n", _DR2, _DR3);
	BootLog::PrintF("DR6=0x%016lx \tDR7=0x%016lx\n", _DR6, _DR7);
	BootLog::Print("\n");
	
	// Loop forever because of Exception
	for (;;) {
		asm volatile (	"cli;\n"
						"hlt;\n");
	}
}

void Interrupt::PrintErrorCode(uint64_t index, uint64_t error_code) {
	
	BootLog::PrintF("Error code=0x%lx (%ld)\n", error_code, error_code);
	
	BootLog::Print("Meaning:\n");
	switch (index) {
		
		case 0x0e:
			if (error_code & 0x01) {
				BootLog::Print(" Protection fault\n");
			} else {
				BootLog::Print(" Page not present\n");
			}
			if (error_code & 0x02) {
				BootLog::Print(" Write access\n");
			} else {
				BootLog::Print(" Read access\n");
			}
			if (error_code & 0x04) {
				BootLog::Print(" User access\n");
			} else {
				BootLog::Print(" Superuser access\n");
			}
			if (error_code & 0x08) {
				BootLog::Print(" Reserved bit set\n");
			}
			if (error_code & 0x10) {
				BootLog::Print(" Instruction fetch\n");
			} else {
				BootLog::Print(" Data access\n");
			}
			if (error_code & 0x20) {
				BootLog::Print(" Protection key violation\n");
			}
			if (error_code & 0x40) {
				BootLog::Print(" Shadow-stack access fault\n");
			}
			if (error_code & 0x8000) {
				BootLog::Print(" SGX violation\n");
			}
			
			break;
			
		default:
			BootLog::Print("---\n");
	}
	BootLog::Print("\n");
	
}

const char *GetExceptionName(uint64_t index) {

	switch (index) {
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
