
#include "gdt.hpp"

GDTD_t GDT::GlobalDescriptorTableDescriptor;
GD_t GDT::GlobalDescriptorTable[GDT_COUNT];
 
GDT::GDT() {
	
}

GDT::~GDT() {
	
}

retval_t GDT::Init(void) {

	// NULL
	GlobalDescriptorTable[GDT_NULL].limit_l = 0;
	GlobalDescriptorTable[GDT_NULL].base_l = 0;
	GlobalDescriptorTable[GDT_NULL].base_m = 0;
	GlobalDescriptorTable[GDT_NULL].access = 0;
	GlobalDescriptorTable[GDT_NULL].limit_h = 0;
	GlobalDescriptorTable[GDT_NULL].flags = 0;
	GlobalDescriptorTable[GDT_NULL].base_h = 0;
	
	// 32Bit
	GlobalDescriptorTable[GDT_CODE32].limit_l = 0xFFFF;
	GlobalDescriptorTable[GDT_CODE32].base_l = 0;
	GlobalDescriptorTable[GDT_CODE32].base_m = 0;
	GlobalDescriptorTable[GDT_CODE32].access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_DATA_CODE |
		GDT_ACCESS_EXECUTE | GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW | GDT_ACCESS_ACCESS;
	GlobalDescriptorTable[GDT_CODE32].limit_h = 0xF;
	GlobalDescriptorTable[GDT_CODE32].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_32BIT;
	GlobalDescriptorTable[GDT_CODE32].base_h = 0;
	
	GlobalDescriptorTable[GDT_DATA32].limit_l = 0xFFFF;
	GlobalDescriptorTable[GDT_DATA32].base_l = 0;
	GlobalDescriptorTable[GDT_DATA32].base_m = 0;
	GlobalDescriptorTable[GDT_DATA32].access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_DATA_CODE |
		GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW | GDT_ACCESS_ACCESS;
	GlobalDescriptorTable[GDT_DATA32].limit_h = 0xF;
	GlobalDescriptorTable[GDT_DATA32].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_32BIT;
	GlobalDescriptorTable[GDT_DATA32].base_h = 0;
	
	// 64Bit
	GlobalDescriptorTable[GDT_CODE64].limit_l = 0xFFFF;
	GlobalDescriptorTable[GDT_CODE64].base_l = 0;
	GlobalDescriptorTable[GDT_CODE64].base_m = 0;
	GlobalDescriptorTable[GDT_CODE64].access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_DATA_CODE |
		GDT_ACCESS_EXECUTE | GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW | GDT_ACCESS_ACCESS;
	GlobalDescriptorTable[GDT_CODE64].limit_h = 0xF;
	GlobalDescriptorTable[GDT_CODE64].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_64BIT;
	GlobalDescriptorTable[GDT_CODE64].base_h = 0;
	
	GlobalDescriptorTable[GDT_DATA64].limit_l = 0xFFFF;
	GlobalDescriptorTable[GDT_DATA64].base_l = 0;
	GlobalDescriptorTable[GDT_DATA64].base_m = 0;
	GlobalDescriptorTable[GDT_DATA64].access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_DATA_CODE |
		GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW | GDT_ACCESS_ACCESS;
	GlobalDescriptorTable[GDT_DATA64].limit_h = 0xF;
	GlobalDescriptorTable[GDT_DATA64].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_64BIT;
	GlobalDescriptorTable[GDT_DATA64].base_h = 0;
	
	// 16Bit
	GlobalDescriptorTable[GDT_CODE16].limit_l = 0xFFFF;
	GlobalDescriptorTable[GDT_CODE16].base_l = 0;
	GlobalDescriptorTable[GDT_CODE16].base_m = 0;
	GlobalDescriptorTable[GDT_CODE16].access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_DATA_CODE |
		GDT_ACCESS_EXECUTE | GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW | GDT_ACCESS_ACCESS;
	GlobalDescriptorTable[GDT_CODE16].limit_h = 0xF;
	GlobalDescriptorTable[GDT_CODE16].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_16BIT;
	GlobalDescriptorTable[GDT_CODE16].base_h = 0;
	
	GlobalDescriptorTable[GDT_DATA16].limit_l = 0xFFFF;
	GlobalDescriptorTable[GDT_DATA16].base_l = 0;
	GlobalDescriptorTable[GDT_DATA16].base_m = 0;
	GlobalDescriptorTable[GDT_DATA16].access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_DATA_CODE |
		GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW | GDT_ACCESS_ACCESS;
	GlobalDescriptorTable[GDT_DATA16].limit_h = 0xF;
	GlobalDescriptorTable[GDT_DATA16].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_16BIT;
	GlobalDescriptorTable[GDT_DATA16].base_h = 0;
	
	
	GlobalDescriptorTableDescriptor.offset = (uint32_t)(void*)GlobalDescriptorTable;
	GlobalDescriptorTableDescriptor.size = GDT_COUNT * GDT_ENTRY_SIZE - 1;
	
	return RETVAL_OK; 
}

void GDT::LoadGDT(void) {
	
	// The Far Jump must be fixed - still don't know how to write it in AT&T syntax
	// Compiler error "undefined reference to $8", but $8 should be an immediate value,
	// not a symbol
	
	asm volatile (
		"lgdt %0;"
		"ljmp %1, $1f;"
		"1:"
		"movw %%ax, %%ds;"
		"movw %%ax, %%es;"
		"movw %%ax, %%fs;"
		"movw %%ax, %%gs;"
		"movw %%ax, %%ss;"
		:  
		: "m" (GlobalDescriptorTableDescriptor), "i" (GDT_CODE32_SEL), "ax" (GDT_DATA32_SEL)
		: 
	);
}
