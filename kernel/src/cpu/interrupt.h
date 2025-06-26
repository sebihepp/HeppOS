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

#ifndef HEADER_INTERRUPT
#define HEADER_INTERRUPT

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <retval.h>

#define INTERRUPT_MAX_COUNT (256)
#define INTERRUPT_MAX_HANDLER (4)

#define IDT_TYPE_GATE	(0xE)
#define IDT_TYPE_TRAP	(0xF)

#define IDT_DPL0		(0x0)
#define IDT_DPL1		(0x1)
#define IDT_DPL2		(0x2)
#define IDT_DPL3		(0x3)

#define IDT_PRESENT		(0x1)
#define IDT_NOT_PRESENT	(0x0)

#define INTERRUPT_PIC_OFFSET	(0x20)


struct IDTEntry_t {
	uint64_t offset_l:16;
	uint64_t segment:16;
	uint64_t ist:3;
	uint64_t reserved0:5;
	uint64_t type:4;
	uint64_t zero:1;
	uint64_t dpl:2;
	uint64_t present:1;
	uint64_t offset_m:16;
	uint64_t offset_h:32;
	uint64_t reserved1:32;
} __attribute__(( packed, aligned(8) )); 

struct IDTD_t {
	uint16_t size;
	uint64_t offset;
} __attribute__(( packed ));

struct CPUState_t {
	
	uint64_t gs;
	uint64_t fs;
	uint64_t es;
	uint64_t ds;
	
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;

	uint64_t rdi;
	uint64_t rsi;
	uint64_t rbp;
	uint64_t reserved;
	
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	
	uint64_t error_code;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __attribute__(( packed ));



typedef void (*ISRHandler_t)(uint64_t pInt, CPUState_t *pState);

extern "C" void ISRHandler(uint64_t pInt, CPUState_t *pState);
extern "C" void ExceptionHandler(uint64_t pInt, CPUState_t *pState);


class CInterrupt {
private:
	CInterrupt() = delete;
	~CInterrupt() = delete;

	static ISRHandler_t mISRHandler[INTERRUPT_MAX_COUNT][INTERRUPT_MAX_HANDLER];
	
	static IDTEntry_t mIDT[INTERRUPT_MAX_COUNT];
	static IDTD_t mIDTD;
	
	static uint64_t mInterruptCount[INTERRUPT_MAX_COUNT];
	
	static void SetIDTEntry(uint8_t pIndex, void *pAddress, uint8_t pType) __attribute__(( nothrow ));
	
	friend void ISRHandler(uint64_t pInt, CPUState_t *pState) __attribute__(( nothrow ));
	friend void ExceptionHandler(uint64_t pInt, CPUState_t *pState) __attribute__(( nothrow ));
	
	static void PrintErrorCode(uint64_t pInt, uint64_t pErrorCode) __attribute__(( nothrow ));
	
	static void LoadIDT(void) __attribute__(( nothrow ));
		
public:
	static ReturnValue_t Init(void) __attribute__(( nothrow ));
	
	static inline void EnableInterrupts(void) __attribute__(( nothrow, always_inline )) {
		asm volatile ("sti;\n");
	}
	
	static inline void DisableInterrupts(void) __attribute__(( nothrow, always_inline )) {
		asm volatile ("cli;\n");
	}
	
	static void RegisterHandler(uint8_t pIndex, ISRHandler_t pHandler) __attribute__(( nothrow ));	
	static void UnregisterHandler(uint8_t pIndex, ISRHandler_t pHandler) __attribute__(( nothrow ));
	
	static uint64_t GetInterruptCount(uint8_t pInt) __attribute__(( nothrow ));
	
};

#endif

