
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#inlcude "multiboot2.hpp"

uint32_t cpuid_max = 0;
uint32_t cpuid_ext_max = 0;
bool longmode_avail = false;

union cpuid_ret_s
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

static inline void cpuid(uint32_t code, cpuid_ret_s &retval)
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

	// Maybe implement some Text output for better diagnosis (check multiboot2 info for framebuffer + ASCII charset)

	cpuid_ret_s retval;
	
	// Check maximum CPUID level
	cpuid(0, retval);
	cpuid_max = retval.a;
	if (retval.a < 0x1) {
		return 1;
	}

	
	// Check maximum extended CPUID level
	cpuid(0x80000000, retval);
	cpuid_ext_max = retval.a;
	if (retval.a < 0x1) {
		return 2;
	}


	//Check for support of long mode
	cpuid(0x80000001, retval);
	if (retval.d & 0x20000000) {
		longmode_avail = true;
	} else {
		return 3;
	}
	
	// Parse modules (relocatable elf)
	
	// check how many address lines are implemented
	
	// setup initial 64bit paging (maybe map kernel space -2GB to the first 2GB in memory?)
	
	// enable long mode
	
	// enable SSE (maybe SSE2 is also standard for LongMode?)
	
	// call kmain of kernel
	
	
	return 0xAA550000;
}
