
#include <cpu/gdt.h>
#include <limine.h>

#include <cstub.h>
#include <retvals.h>

GDTD_t GDT::mGlobalDescriptorTableDescriptor;
GDTEntry_t GDT::mGlobalDescriptorTable[GDT_TOTAL_COUNT];
TSS_t GDT::mTaskStateSegment;


GDT::GDT() {
	
}

GDT::~GDT() {
	
}

retval_t GDT::Init(void) {

	memset(mGlobalDescriptorTable, 0, GDT_TOTAL_COUNT * GDT_ENTRY_SIZE);
	memset(&mTaskStateSegment, 0, sizeof(mTaskStateSegment));
	
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
		GDT_ACCESS_EXECUTE | GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW;
	mGlobalDescriptorTable[GDT_CODE64].limit_h = 0;
	mGlobalDescriptorTable[GDT_CODE64].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_64BIT;
	mGlobalDescriptorTable[GDT_CODE64].base_h = 0;
	
	// 64Bit Data
	mGlobalDescriptorTable[GDT_DATA64].limit_l = 0;
	mGlobalDescriptorTable[GDT_DATA64].base_l = 0;
	mGlobalDescriptorTable[GDT_DATA64].base_m = 0;
	mGlobalDescriptorTable[GDT_DATA64].access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_DATA_CODE |
		GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW;
	mGlobalDescriptorTable[GDT_DATA64].limit_h = 0;
	mGlobalDescriptorTable[GDT_DATA64].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_64BIT;
	mGlobalDescriptorTable[GDT_DATA64].base_h = 0;	
	
	// TSS
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->limit_l = sizeof(mTaskStateSegment) & 0xFFFF;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->base_l = reinterpret_cast<uint64_t>(&mTaskStateSegment) & 0xFFFF;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->base_m = (reinterpret_cast<uint64_t>(&mTaskStateSegment) >> 16) & 0xFF;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_TSS_SEGMENT | GDT_ACCESS_TSS_TYPE64;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->limit_h = sizeof(mTaskStateSegment) >> 16 & 0xF;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_64BIT;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->base_h = (reinterpret_cast<uint64_t>(&mTaskStateSegment) >> 24) & 0xFF;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->base_vh = (reinterpret_cast<uint64_t>(&mTaskStateSegment) >> 32) & 0xFFFFFFFF;
	
	
	// GDTD
	mGlobalDescriptorTableDescriptor.base = reinterpret_cast<uint64_t>(mGlobalDescriptorTable);
	mGlobalDescriptorTableDescriptor.limit = (GDT_TOTAL_COUNT * GDT_ENTRY_SIZE) - 1;
	
	
	return RETVAL_OK; 
}

void GDT::LoadGDT(void) {
	
	asm volatile (
		//"xchgw %%bx, %%bx;\n" //Magic breakpoint in bochs for debugging
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

void GDT::LoadTSS(void) {
	
		asm volatile (
		//"xchgw %%bx, %%bx;\n" //Magic breakpoint in bochs for debugging
		"mov %0, %%ax;\n"
		"ltr %%ax;\n"
		:  
		: "i" (GDT_TSS_SEL)
		: "ax"
	);
}

TSS_t *GDT::GetTSS(void) {
	return &mTaskStateSegment;
}

uint16_t GDT::GetSelector(uint64_t pSelector) {
	return pSelector;
}
