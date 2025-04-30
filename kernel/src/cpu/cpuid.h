
#ifndef CPUID_HEADER
#define CPUID_HEADER

#include <stddef.h>
#include <stdint.h>

struct cpuid_retval_t
{
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
} __attribute__((packed));

static inline void cpuid(const uint32_t pCode, cpuid_retval_t &pRetval)  __attribute__ (( nothrow, always_inline ));
static inline void cpuid(const uint32_t pCode, cpuid_retval_t &pRetval)
{
	asm volatile 
	(
		"cpuid" :
		"=a" (pRetval.eax), "=b" (pRetval.ebx), "=c" (pRetval.ecx), "=d" (pRetval.edx) :
		"a" (pCode) :
	);	
}

#endif

