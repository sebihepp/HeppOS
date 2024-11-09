
#ifndef MSR_HEADER
#define MSR_HEADER

#include <stddef.h>
#include <stdint.h>

#define MSR_PAT (0x277)
#define MSR_EFER (0xC0000080)

static inline void ReadMSR(uint32_t pMSR, uint32_t *pLow, uint32_t *pHigh)
{
	asm volatile 
	(
		"mov %0, %%ecx;\n"
		"rdmsr;\n"
		: "=a" (*pLow), "=d" (*pHigh)
		: "m" (pMSR)
		: "ecx"
	);	
}

static inline void WriteMSR(uint32_t pMSR, uint32_t pLow, uint32_t pHigh)
{
	asm volatile 
	(
		"mov %0, %%eax;\n"
		"mov %1, %%edx;\n"
		"mov %2, %%ecx;\n"
		"wrmsr;\n"
		:
		: "m" (pLow), "m" (pHigh), "m" (pMSR)
		: "eax", "ecx", "edx"
	);	
}

#endif
