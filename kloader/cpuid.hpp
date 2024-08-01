
#ifndef CPUID_HEADER
#define CPUID_HEADER

#include <stddef.h>
#include <stdint.h>

union cpuid_retval_t
{
	uint32_t reg[4];
	struct 
	{
		uint32_t eax;
		uint32_t ebx;
		uint32_t ecx;
		uint32_t edx;
	} __attribute__((packed));
} __attribute__((packed));

static inline void cpuid(uint32_t code, cpuid_retval_t &retval)
{
	asm volatile 
	(
		"cpuid" :
		"=a" (retval.eax), "=b" (retval.ebx), "=c" (retval.ecx), "=d" (retval.edx) :
		"a" (code) :
	);	
}

#endif
