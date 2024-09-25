
#include <gdt.h>
#include <limine.h>

#include <retvals.h>

GDTD_t GDT::mGlobalDescriptorTableDescriptor;
GD_t GDT::mGlobalDescriptorTable[GDT_COUNT];
 
GDT::GDT() {
	
}

GDT::~GDT() {
	
}

retval_t GDT::Init(void) {

	
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
	mGlobalDescriptorTableDescriptor.base = (uint64_t)mGlobalDescriptorTable;
	mGlobalDescriptorTableDescriptor.limit = GDT_COUNT * GDT_ENTRY_SIZE - 1;
	
	
	return RETVAL_OK; 
}

void GDT::LoadGDT(void) {
	
	asm volatile (
		//"xchgw %%bx, %%bx;\n"
		"lgdt %0;\n"
		"movq %%rsp, %%rbx;\n"		//Save RSP
		"movq %2, %%rax;\n" 			
		"pushq %%rax;\n"			//Return SS
		"pushq %%rbx;\n" 			//Restore Return RSP
		"movq %1, %%rax;\n"
		"pushq %%rax;\n"			//Return CS
		"movq $_ReloadCS, %%rax;\n"
		"pushq %%rax;\n"			//Return RIP
		"lretq\n"
		"_ReloadCS:;\n"
		"movq %2, %%rbx;\n"
		"mov %%bx, %%ds;\n"
		"mov %%bx, %%es;\n"
		"mov %%bx, %%fs;\n"
		"mov %%bx, %%gs;\n"
		"mov %%bx, %%ss;\n"
		:  
		: "m" (mGlobalDescriptorTableDescriptor), "i" (GDT_CODE64_SEL), "i" (GDT_DATA64_SEL)
		: "rbx", "rax"
	);
}
