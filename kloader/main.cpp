
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "multiboot2.hpp"
#include "console.hpp"
#include "retvals.hpp"

union cpuid_retval_t
{
	uint32_t x[4];
	struct 
	{
		uint32_t a;
		uint32_t b;
		uint32_t c;
		uint32_t d;
	} __attribute__((packed));
} __attribute__((packed));

static inline void cpuid(uint32_t code, cpuid_retval_t &retval)
{
	asm volatile 
	(
		"cpuid" :
		"=a" (retval.a), "=b" (retval.b), "=c" (retval.c), "=d" (retval.d) :
		"a" (code) :
	);	
}

extern "C" uint32_t main(uint32_t magic, multiboot2_info_t *mb2_info)
{
	retval_t retval;
	
	if (magic != 0x36D76289) {
		return RETVAL_ERROR_MB2_MAGIC;
	}
	
	retval = Console::Init(mb2_info);
	if (retval != RETVAL_OK) {
		return retval;
	}
	
	Console::Clear();
	Console::SetTitleText("HeppOS - kloader");
	
	// Maybe implement some Text output for better diagnosis (check multiboot2 info for framebuffer + ASCII charset)

	
	cpuid_retval_t cpuid_retval;
	
	Console::Print("Checking for LongMode - ");
	// Check maximum extended CPUID level	
	cpuid(0x80000000, cpuid_retval);
	if (cpuid_retval.a < 0x1) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_LONGMODE;
	}

	//Check for support of long mode
	cpuid(0x80000001, cpuid_retval);
	if ((cpuid_retval.d & 0x20000000) == 0) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_LONGMODE;
	}
	Console::Print("OK\n");
	
	// Parse modules (relocatable elf)
	
	// check how many address lines are implemented
	
	// setup initial 64bit paging (maybe map kernel space -2GB to the first 2GB in memory?)
	
	// enable long mode
	
	// enable SSE (maybe SSE2 is also standard for LongMode?)
	
	// call kmain of kernel
	
	Console::Print("Finished!");
	return RETVAL_OK;
}
