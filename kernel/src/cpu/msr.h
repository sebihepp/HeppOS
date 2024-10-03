
#ifndef MSR_HEADER
#define MSR_HEADER

#include <stddef.h>
#include <stdint.h>

#define MSR_PAT (0x277)

static inline void ReadMSR(uint32_t pMSR, uint32_t *pLow, uint32_t *pHigh)
{
	asm volatile 
	(
		"rdmsr" :
		"=a" (*pLow), "=d" (*pHigh) :
		"c" (pMSR) :
	);	
}

static inline void WriteMSR(uint32_t pMSR, uint32_t pLow, uint32_t pHigh)
{
	asm volatile 
	(
		"wrmsr" :
		:
		"a" (pLow), "d" (pHigh), "c" (pMSR) :
	);	
}

#endif
