
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "multiboot2.hpp"
#include "console.hpp"
#include "retvals.hpp"
#include "gdt.hpp"
#include "paging.hpp"
#include "cxxstub.h"
#include "cpuid.hpp"


char* itoa(int num, char* str, int base);

extern "C" uint32_t main(uint32_t magic, multiboot2_info_t *mb2_info)
{
	retval_t retval;
	cpuid_retval_t cpuid_retval;
	uint32_t _padr_bits = 0;
	uint32_t _vadr_bits = 0;
	char _temp_text[16];
	memset(_temp_text, 0, 16);
	
	if (magic != 0x36D76289) {
		return RETVAL_ERROR_MB2_MAGIC;
	}
	
	retval = Console::Init(mb2_info);
	if (retval != RETVAL_OK) {
		return retval;
	}
	
	Console::Clear();
	Console::SetTitleText("HeppOS - kloader");
	
	// Check for LongMode
	Console::Print("Checking for LongMode - ");
	// Check maximum extended CPUID level	
	cpuid(0x80000000, cpuid_retval);
	if (cpuid_retval.eax < 0x1) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_LONGMODE;
	}

	//Check for support of long mode
	cpuid(0x80000001, cpuid_retval);
	if ((cpuid_retval.edx & 0x20000000) == 0) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_LONGMODE;
	}
	Console::Print("OK\n");
	
	// Check for PAE
	Console::Print("Checking for PAE - ");	
	cpuid(0x80000001, cpuid_retval);
	if ((cpuid_retval.edx & 0x00000040) == 0) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_PAE;
	}
	Console::Print("OK\n");
		
	
	// Check for SSE
	Console::Print("Checking for SSE - ");	
	cpuid(0x00000001, cpuid_retval);
	if ((cpuid_retval.edx & 0x02000000) == 0) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_SSE;
	}
	Console::Print("OK\n");
	
	// Check for SSE2
	Console::Print("Checking for SSE2 - ");	
	cpuid(0x00000001, cpuid_retval);
	if ((cpuid_retval.edx & 0x04000000) == 0) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_SSE2;
	}
	Console::Print("OK\n");

	
	// Check for FXSAVE/FXRSTOR
	Console::Print("Checking for FXSAVE/FXRSTOR - ");	
	cpuid(0x00000001, cpuid_retval);
	if ((cpuid_retval.edx & 0x01000000) == 0) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_NO_FXSAVE;
	}
	Console::Print("OK\n");	
	
	// enable SSE and SSE2 (standard for LongMode)
	Console::Print("Enabling SSE and SSE2 - ");
	asm volatile (
		"movl %%cr0, %%eax;\n"
		"andw $0xFFFB, %%ax;\n"		//clear EM bit
		"orw $0x2, %%ax;\n"			//set MP bit
		"movl %%eax, %%cr0;\n"
		
		"movl %%cr4, %%eax;\n"
		"orw $0x0600, %%ax;\n"		//set OSFXSR and OSXMMEXCPT bits
		"movl %%eax, %%cr4;\n"
		:
		:
		: "eax"
	);
	Console::Print("OK\n");
	
	
	// check for VA57 (57bit Paging)
	Console::Print("Checking for 5-level-paging - ");
	cpuid(0x00000000, cpuid_retval);
	if (cpuid_retval.eax < 7) {
		Console::Print("NO\n");
	} else {
		cpuid(0x00000007, cpuid_retval);
		if ((cpuid_retval.ecx & 0x00010000) == 0) {
			Console::Print("NO\n");
		} else {
			Console::Print("YES\n");
		}
	}
	
	// check for 1GB Pages (PG1G)
	Console::Print("Checking for 1GB pages - ");
	cpuid(0x80000000, cpuid_retval);
	if (cpuid_retval.eax < 1) {
		Console::Print("ERROR\n");
	} else {
		cpuid(0x80000001, cpuid_retval);
		if ((cpuid_retval.edx & 0x04000000) == 0) {
			Console::Print("ERROR\n");
			return RETVAL_ERROR_NO_1GB_PAGES;
		} else {
			Console::Print("OK\n");
		}
	}

	// check for global Pages (PGE)
	Console::Print("Checking for global pages - ");
	cpuid(0x00000000, cpuid_retval);
	if (cpuid_retval.eax < 1) {
		Console::Print("ERROR\n");
	} else {
		cpuid(0x00000001, cpuid_retval);
		if ((cpuid_retval.edx & 0x00000080) == 0) {
			Console::Print("ERROR\n");
			return RETVAL_ERROR_NO_GLOBAL_PAGES;
		} else {
			Console::Print("OK\n");
		}
	}
	
	// check how many physical address lines are implemented
	Console::Print("Supported physical addresses - ");
	cpuid(0x80000000, cpuid_retval);
	if (cpuid_retval.eax < 8) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_GENERAL;
	} else {
		cpuid(0x80000008, cpuid_retval);
		_padr_bits = cpuid_retval.eax & 0xFF;
		if (_padr_bits < 32) {
			Console::Print("ERROR\n");
			return RETVAL_ERROR_GENERAL;
		} else {
			Console::Print(itoa(_padr_bits, _temp_text, 10));
			Console::Print("bit\n");
		}
	}

	// check how many virtual address lines are implemented
	Console::Print("Supported virtual addresses - ");
	cpuid(0x80000000, cpuid_retval);
	if (cpuid_retval.eax < 8) {
		Console::Print("ERROR\n");
		return RETVAL_ERROR_GENERAL;
	} else {
		cpuid(0x80000008, cpuid_retval);
		_vadr_bits = (cpuid_retval.eax >> 8) & 0xFF;
		if (_vadr_bits < 32) {
			Console::Print("ERROR\n");
			return RETVAL_ERROR_GENERAL;
		} else {
			Console::Print(itoa(_vadr_bits, _temp_text, 10));
			Console::Print("bit\n");
		}
	}
	
	
	// setup initial 64bit paging (maybe map kernel space -2GB to the first 2GB in memory?)
	Console::Print("Setup Paging - ");
	retval = Paging::Init();
	if (retval != RETVAL_OK) {
		Console::Print("ERROR!\n");	
		return retval;
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
	
	
	// enable long mode and paging - 48bit
	Console::Print("Enabling Long Mode - ");	
	asm volatile (
		//"xchgw %%bx, %%bx;\n" //Breakpoint for debugging
		
		//Save PML4 Table on stack
		"pushl %%edx;\n"
		
		//Disable Paging
		"movl %%cr0, %%eax;\n"
		"andl $0x7FFFFFFF, %%eax;\n"
		"movl %%eax, %%cr0;\n"
	
		//Enable PAE
		"movl %%cr4, %%eax;\n"
		"orl $0x00000020, %%eax;\n"
		"movl %%eax, %%cr4;\n"
		
		//Enable Long Mode
		"mov $0xC0000080, %%ecx;\n"
		"rdmsr;\n"
		"or $0x00000100, %%eax;\n"
		"wrmsr;\n"
		
		//Set PML4T
		"popl %%edx;\n"	//Restore PML4 Table from stack
		"movl %%edx, %%cr3;\n"
		
		//Enable Paging
		"movl %%cr0, %%eax;\n"
		"orl $0x80000000, %%eax;\n"
		"movl %%eax, %%cr0;\n"
		
		// Jump to update CS
		"ljmp %1, $1f;\n"
		"1:"
		"movw %2, %%ax;\n"
		"movw %%ax, %%ds;\n"
		"movw %%ax, %%es;\n"
		"movw %%ax, %%fs;\n"
		"movw %%ax, %%gs;\n"
		"movw %%ax, %%ss;\n"
		
		:
		: "d" (Paging::GetPML4T()), "i" (GDT_CODE32_SEL), "i" (GDT_DATA32_SEL)
		: "eax", "ecx", "bx"
	);
	Console::Print("OK\n");
	
	// Parse modules (relocatable elf)
	
	// jump to kmain of kernel
	
	Console::Print("Finished!");
	return RETVAL_OK;
}


void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char* itoa(int num, char* str, int base)
{
	int i = 0;
	bool isNegative = false;
	
	/* Handle 0 explicitly, otherwise empty string is
	* printed for 0 */
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
	
	// In standard itoa(), negative numbers are handled
	// only with base 10. Otherwise numbers are
	// considered unsigned.
	if (num < 0 && base == 10) {
		isNegative = true;
		num = -num;
	}
	
	// Process individual digits
	while (num != 0) {
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}
	
	// If number is negative, append '-'
	if (isNegative)
		str[i++] = '-';
	
	str[i] = '\0'; // Append string terminator
	
	// Reverse the string
	reverse(str, i);
 
	return str;
}


 

