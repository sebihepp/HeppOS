
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "multiboot2.hpp"
#include "console.hpp"
#include "retvals.hpp"
#include "gdt.hpp"

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
	
	cpuid_retval_t cpuid_retval;
	
	// Check for LongMode
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
	
	// Check for PAE
	Console::Print("Checking for PAE - ");	
	cpuid(0x80000001, cpuid_retval);
	if ((cpuid_retval.d & 0x00000040) == 0) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_PAE;
	}
	Console::Print("OK\n");
	
	// check how many address lines are implemented
	Console::Print("Supported virtual addresses - ");
	cpuid(0x00000000, cpuid_retval);
	if (cpuid_retval.a < 7) {
		Console::Print("48bit\n");
	} else {
		cpuid(0x00000007, cpuid_retval);
		if ((cpuid_retval.c & 0x00010000) == 0) {
			Console::Print("48bit\n");
		} else {
			Console::Print("57bit\n");
		}
	}
	
	// Check for SSE
	Console::Print("Checking for SSE - ");	
	cpuid(0x00000001, cpuid_retval);
	if ((cpuid_retval.d & 0x02000000) == 0) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_SSE;
	}
	Console::Print("OK\n");
	
	// Check for SSE2
	Console::Print("Checking for SSE2 - ");	
	cpuid(0x00000001, cpuid_retval);
	if ((cpuid_retval.d & 0x04000000) == 0) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_SSE2;
	}
	Console::Print("OK\n");

	
	// Check for FXSAVE/FXRSTOR
	Console::Print("Checking for FXSAVE/FXRSTOR - ");	
	cpuid(0x00000001, cpuid_retval);
	if ((cpuid_retval.d & 0x01000000) == 0) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_FXSAVE;
	}
	Console::Print("OK\n");
	
	
	// Setup GDT
	Console::Print("Setup GDT - ");
	retval = GDT::Init();
	if (retval != RETVAL_OK) {
		Console::Print("ERROR\n");
		return retval;
	}
	Console::Print("OK\n");
	
	// Load GDT
	Console::Print("Load GDT - ");
	GDT::LoadGDT();
	Console::Print("OK\n");
	
	// enable SSE and SSE2 (standard for LongMode)
	
	
	// setup initial 64bit paging (maybe map kernel space -2GB to the first 2GB in memory?)
	
	// enable long mode - 48bit
	
	
	
	// Parse modules (relocatable elf)
	
	// jump to kmain of kernel
	
	Console::Print("Finished!");
	return RETVAL_OK;
}
