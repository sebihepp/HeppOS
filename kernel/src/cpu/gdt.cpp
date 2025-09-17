/* 
    Copyright (C) 2025  Sebastian Hepp aka sebihepp

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cpu/gdt.h>
#include <limine.h>
#include <kstring.h>


GDTD_t CGDT::mGlobalDescriptorTableDescriptor;
GDTEntry_t CGDT::mGlobalDescriptorTable[GDT_TOTAL_COUNT];
TSS_t CGDT::mTaskStateSegment;


ReturnValue_t CGDT::Init(void) {

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
	
	// TSS Entry
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->limit_l = sizeof(mTaskStateSegment) & 0xFFFF;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->base_l = reinterpret_cast<uint64_t>(&mTaskStateSegment) & 0xFFFF;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->base_m = (reinterpret_cast<uint64_t>(&mTaskStateSegment) >> 16) & 0xFF;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_TSS_SEGMENT | GDT_ACCESS_TSS_TYPE64;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->limit_h = sizeof(mTaskStateSegment) >> 16 & 0xF;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_64BIT;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->base_h = (reinterpret_cast<uint64_t>(&mTaskStateSegment) >> 24) & 0xFF;
	((GDTSystemEntry_t*)&(mGlobalDescriptorTable[GDT_TSS]))->base_vh = (reinterpret_cast<uint64_t>(&mTaskStateSegment) >> 32) & 0xFFFFFFFF;
	
	// Setup TSS
	mTaskStateSegment.RSP0 = 0;
	mTaskStateSegment.RSP1 = 0;
	mTaskStateSegment.RSP2 = 0;
	mTaskStateSegment.IST1 = 0;
	mTaskStateSegment.IST2 = 0;
	mTaskStateSegment.IST3 = 0;
	mTaskStateSegment.IST4 = 0;
	mTaskStateSegment.IST5 = 0;
	mTaskStateSegment.IST6 = 0;
	mTaskStateSegment.IST7 = 0;
	mTaskStateSegment.IOPB_offset = 104;
	
	
	// GDTD
	mGlobalDescriptorTableDescriptor.base = reinterpret_cast<uint64_t>(mGlobalDescriptorTable);
	mGlobalDescriptorTableDescriptor.limit = (GDT_TOTAL_COUNT * GDT_ENTRY_SIZE) - 1;
	
	LoadGDT();
	LoadTSS();
	
	return RETVAL_OK; 
}

void CGDT::LoadGDT(void) {
	
	asm volatile (
		//"xchgw %%bx, %%bx;\n" //Magic breakpoint in bochs for debugging
		"lgdt %0;\n"
		"movq %%rsp, %%rbx;\n"		//Save RSP
		"movq %2, %%rax;\n" 			
		"pushq %%rax;\n"			//Return SS
		"pushq %%rbx;\n" 			//Restore Return RSP
		"movq %1, %%rax;\n"
		"pushq %%rax;\n"			//Return CS
		"leaq _ReloadCS(%%rip), %%rax;\n"
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

void CGDT::LoadTSS(void) {
	
		asm volatile (
		//"xchgw %%bx, %%bx;\n" //Magic breakpoint in bochs for debugging
		"mov %0, %%ax;\n"
		"ltr %%ax;\n"
		:  
		: "i" (GDT_TSS_SEL)
		: "ax"
	);
}

TSS_t *CGDT::GetTSS(void) {
	return &mTaskStateSegment;
}

uint16_t CGDT::GetSelector(uint64_t pSelector) {
	return pSelector;
}

