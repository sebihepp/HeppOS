
#ifndef HEADER_INTERRUPT
#define HEADER_INTERRUPT

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <retvals.h>


#define IDT_TYPE_GATE	(0xE)
#define IDT_TYPE_TRAP	(0xF)

#define IDT_DPL0		(0x0)
#define IDT_DPL1		(0x1)
#define IDT_DPL2		(0x2)
#define IDT_DPL3		(0x3)

#define IDT_PRESENT		(0x1)
#define IDT_NOT_PRESENT	(0x0)


typedef void (*ISRHandler_t)(void);


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
} __attribute__((packed)); 

struct IDTD_t {
	uint16_t size;
	uint64_t offset;
} __attribute__((packed)); 

struct CPUState_t {
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
} __attribute__((packed));


class Interrupt {
private:
	Interrupt();
	~Interrupt();

	static IDTEntry_t mIDT[256];
	static IDTD_t mIDTD;
	
	static void RegisterHandler(uint8_t pIndex, ISRHandler_t pHandler);
	static void RegisterException(uint8_t pIndex, ISRHandler_t pHandler);
	
public:
	static retval_t Init(void);
	
	static void LoadIDT(void);
	
};

#endif