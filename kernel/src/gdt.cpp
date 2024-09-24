
#include <gdt.h>
#include <limine.h>

#include <retvals.h>

GDTD_t GDT::mGlobalDescriptorTableDescriptor;
GD_t GDT::mGlobalDescriptorTable[GDT_COUNT];
 
GDT::GDT() {
	
}

GDT::~GDT() {
	
}

retval_t GDT::Init(limine_hhdm_response *pHHDMResponse) {

	if (pHHDMResponse == NULL) {
		return RETVAL_ERROR_NO_HHDM;
	}
	
	// NULL descriptor
	mGlobalDescriptorTable[GDT_NULL].limit_l = 0;
	mGlobalDescriptorTable[GDT_NULL].base_l = 0;
	mGlobalDescriptorTable[GDT_NULL].base_m = 0;
	mGlobalDescriptorTable[GDT_NULL].access = 0;
	mGlobalDescriptorTable[GDT_NULL].limit_h = 0;
	mGlobalDescriptorTable[GDT_NULL].flags = 0;
	mGlobalDescriptorTable[GDT_NULL].base_h = 0;
	
	
	// 64Bit Code
	mGlobalDescriptorTable[GDT_CODE64].limit_l = 0;
	mGlobalDescriptorTable[GDT_CODE64].base_l = 0;
	mGlobalDescriptorTable[GDT_CODE64].base_m = 0;
	mGlobalDescriptorTable[GDT_CODE64].access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_DATA_CODE |
		GDT_ACCESS_EXECUTE | GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW | GDT_ACCESS_ACCESSED;
	mGlobalDescriptorTable[GDT_CODE64].limit_h = 0;
	mGlobalDescriptorTable[GDT_CODE64].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_64BIT;
	mGlobalDescriptorTable[GDT_CODE64].base_h = 0;
	
	// 64Bit Data
	mGlobalDescriptorTable[GDT_DATA64].limit_l = 0;
	mGlobalDescriptorTable[GDT_DATA64].base_l = 0;
	mGlobalDescriptorTable[GDT_DATA64].base_m = 0;
	mGlobalDescriptorTable[GDT_DATA64].access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_DATA_CODE |
		GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW | GDT_ACCESS_ACCESSED;
	mGlobalDescriptorTable[GDT_DATA64].limit_h = 0;
	mGlobalDescriptorTable[GDT_DATA64].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_64BIT;
	mGlobalDescriptorTable[GDT_DATA64].base_h = 0;	
	
	// GDTD
	mGlobalDescriptorTableDescriptor.offset = (uint64_t)mGlobalDescriptorTable;
	mGlobalDescriptorTableDescriptor.offset -= pHHDMResponse->offset;
	mGlobalDescriptorTableDescriptor.size = GDT_COUNT * GDT_ENTRY_SIZE - 1;
	
	
	return RETVAL_OK; 
}

void GDT::LoadGDT(void) {
	
	asm volatile (
"xchgw %%bx, %%bx;\n"
		"lgdt %0;\n"
		"pushq %1;\n"
		"pushq $1f;\n"
		"retfq\n"
		"1:\n"
		"mov %2, %%ds;\n"
		"mov %2, %%es;\n"
		"mov %2, %%fs;\n"
		"mov %2, %%gs;\n"
		"mov %2, %%ss;\n"
		:  
		: "m" (mGlobalDescriptorTableDescriptor), "i" (GDT_CODE64_SEL), "r" (GDT_DATA64_SEL)
		: "ebx"
	);
}
