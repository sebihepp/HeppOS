
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "multiboot2.hpp"
#include "video.hpp"
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
	
	retval = Video::init(mb2_info);
	if (retval != RETVAL_OK) {
		return retval;
	}
	
	for (uint32_t i = 0; i < 256; ++i) {
		uint32_t linear = i * 8;
		uint32_t x = linear % Video::getWidth();
		uint32_t y = (linear / Video::getWidth()) * 16;
		if (y >= Video::getHeight())
			break;
		Video::print_char(i, x, y);
	}
	
	
	// Maybe implement some Text output for better diagnosis (check multiboot2 info for framebuffer + ASCII charset)

	// Check maximum extended CPUID level	
	cpuid_retval_t cpuid_retval;
	cpuid(0x80000000, cpuid_retval);
	if (cpuid_retval.a < 0x1) {
		return RETVAL_ERROR_NO_LONGMODE;
	}

	//Check for support of long mode
	cpuid(0x80000001, cpuid_retval);
	if ((cpuid_retval.d & 0x20000000) == 0) {
		return RETVAL_ERROR_NO_LONGMODE;
	}
	
	// Parse modules (relocatable elf)
	
	// check how many address lines are implemented
	
	// setup initial 64bit paging (maybe map kernel space -2GB to the first 2GB in memory?)
	
	// enable long mode
	
	// enable SSE (maybe SSE2 is also standard for LongMode?)
	
	// call kmain of kernel
	
	
	return RETVAL_OK;
}
