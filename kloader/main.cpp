
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

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

uint32_t main(uint32_t magic, void *mb2_info)
{

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
	
	// setup initial 64bit paging
	
	// enable long mode
	
	// call kmain of kernel
	
	
	return 0xAA550000;
}
