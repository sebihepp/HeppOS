
#include "paging.hpp"
#include "cstub.h"

#include "console.hpp"
#include "msr.hpp"

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

 
PML4Table_t Paging::PML4T;
PML3Table_t Paging::PML3THigh;
PML3Table_t Paging::PML3TLow;

Paging::Paging() {
}

Paging::~Paging() {
}
	

retval_t Paging::Init(void) {

	uint32_t _padr_bits = 0;
	cpuid_retval_t cpuid_retval;
	// check how many physical address lines are implemented
	cpuid(0x80000000, cpuid_retval);
	if (cpuid_retval.a < 8) {
		return RETVAL_ERROR_GENERAL;
	} else {
		cpuid(0x80000008, cpuid_retval);
		_padr_bits = cpuid_retval.a & 0xFF;
		if (_padr_bits < 32) {
			return RETVAL_ERROR_GENERAL;
		}
	}
	
	// Setup the PAT (Page Attribute Table)
	WriteMSR(MSR_PAT, 0x00000100, 0x07060504);
	
	// Clear all Paging structures
	memset(&PML4T, 0, 4096);
	memset(&PML3THigh, 0, 4096);
	memset(&PML3TLow, 0, 4096);

	// Setup PML4 Table
	PML4T.entry[0].present = 1;
	PML4T.entry[0].write = 1;
	PML4T.entry[0].user = 0;
	PML4T.entry[0].write_through = 0;
	PML4T.entry[0].cache_disable = 0;
	PML4T.entry[0].accessed = 0;
	PML4T.entry[0].base = ((uintptr_t)&PML3TLow) >> 12;
	PML4T.entry[0].execute_disable = 0;
	
	PML4T.entry[511].present = 1;
	PML4T.entry[511].write = 1;
	PML4T.entry[511].user = 0;
	PML4T.entry[511].write_through = 0;
	PML4T.entry[511].cache_disable = 0;
	PML4T.entry[511].accessed = 0;
	PML4T.entry[511].base = ((uintptr_t)&PML3THigh) >> 12;
	PML4T.entry[511].execute_disable = 0;

	// Setup PML3Low Table
	PML3TLow.entry1GB[0].present = 1;
	PML3TLow.entry1GB[0].write = 1;
	PML3TLow.entry1GB[0].user = 0;
	PML3TLow.entry1GB[0].write_through = 0;
	PML3TLow.entry1GB[0].cache_disable = 1;
	PML3TLow.entry1GB[0].accessed = 0;
	PML3TLow.entry1GB[0].size = 1;
	PML3TLow.entry1GB[0].global = 0;
	PML3TLow.entry1GB[0].page_attribute_table = 1;
	PML3TLow.entry1GB[0].base = (uint64_t)0x0;
	PML3TLow.entry1GB[0].execute_disable = 0;
	
	PML3TLow.entry1GB[1].present = 1;
	PML3TLow.entry1GB[1].write = 1;
	PML3TLow.entry1GB[1].user = 0;
	PML3TLow.entry1GB[1].write_through = 0;
	PML3TLow.entry1GB[1].cache_disable = 1;
	PML3TLow.entry1GB[1].accessed = 0;
	PML3TLow.entry1GB[1].size = 1;
	PML3TLow.entry1GB[1].global = 0;
	PML3TLow.entry1GB[1].page_attribute_table = 1;
	PML3TLow.entry1GB[1].base = ((uint64_t)1024*1024*1024) >> 30;
	PML3TLow.entry1GB[1].execute_disable = 0;
	

	// Setup PML3High Table
	PML3THigh.entry1GB[510].present = 1;
	PML3THigh.entry1GB[510].write = 1;
	PML3THigh.entry1GB[510].user = 0;
	PML3THigh.entry1GB[510].write_through = 0;
	PML3THigh.entry1GB[510].cache_disable = 1;
	PML3THigh.entry1GB[510].accessed = 0;
	PML3THigh.entry1GB[510].size = 1;
	PML3THigh.entry1GB[510].global = 0;
	PML3THigh.entry1GB[510].page_attribute_table = 1;
	PML3THigh.entry1GB[510].base = (uint64_t)0x0;
	PML3THigh.entry1GB[510].execute_disable = 0;

	PML3THigh.entry1GB[511].present = 1;
	PML3THigh.entry1GB[511].write = 1;
	PML3THigh.entry1GB[511].user = 0;
	PML3THigh.entry1GB[511].write_through = 0;
	PML3THigh.entry1GB[511].cache_disable = 1;
	PML3THigh.entry1GB[511].accessed = 0;
	PML3THigh.entry1GB[511].size = 1;
	PML3THigh.entry1GB[511].global = 0;
	PML3THigh.entry1GB[511].page_attribute_table = 1;
	PML3THigh.entry1GB[511].base = ((uint64_t)1024*1024*1024) >> 30;
	PML3THigh.entry1GB[511].execute_disable = 0;
	
	
	// Setup Page Tables for Framebuffer
	uint64_t _framebuffer = (uint64_t)Console::GetFramebufferAddress();
	_framebuffer &= 0xFFFFFFFF;
	uint32_t PML3Entry = (_framebuffer >> 30) & 0x1FF;
	
	PML3TLow.entry1GB[PML3Entry].present = 1;
	PML3TLow.entry1GB[PML3Entry].write = 1;
	PML3TLow.entry1GB[PML3Entry].user = 0;
	PML3TLow.entry1GB[PML3Entry].write_through = 1;
	PML3TLow.entry1GB[PML3Entry].cache_disable = 0;
	PML3TLow.entry1GB[PML3Entry].accessed = 0;
	PML3TLow.entry1GB[PML3Entry].size = 1;
	PML3TLow.entry1GB[PML3Entry].global = 0;
	PML3TLow.entry1GB[PML3Entry].page_attribute_table = 0;
	PML3TLow.entry1GB[PML3Entry].base = _framebuffer >> 30;
	PML3TLow.entry1GB[PML3Entry].execute_disable = 0;
	
	
	return RETVAL_OK;
}

const void* Paging::GetPML4T(void) {
	return &PML4T;	
}
