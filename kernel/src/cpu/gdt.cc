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
#include <limine_stub.h>
#include <kstring.h>


GDTDescriptor GDT::gdtd_;
GDTEntry GDT::gdt_[GDT_TOTAL_COUNT];
TSS GDT::tss_;


ReturnValue GDT::Init(void) {

	memset(gdt_, 0, GDT_TOTAL_COUNT * GDT_ENTRY_SIZE);
	memset(&tss_, 0, sizeof(tss_));
	
	// NULL descriptor
	gdt_[GDT_NULL].limit_l = 0;
	gdt_[GDT_NULL].base_l = 0;
	gdt_[GDT_NULL].base_m = 0;
	gdt_[GDT_NULL].access = 0;
	gdt_[GDT_NULL].limit_h = 0;
	gdt_[GDT_NULL].flags = 0;
	gdt_[GDT_NULL].base_h = 0;
	
	
	// 64Bit Code
	gdt_[GDT_CODE64].limit_l = 0;
	gdt_[GDT_CODE64].base_l = 0;
	gdt_[GDT_CODE64].base_m = 0;
	gdt_[GDT_CODE64].access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_DATA_CODE |
		GDT_ACCESS_EXECUTE | GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW;
	gdt_[GDT_CODE64].limit_h = 0;
	gdt_[GDT_CODE64].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_64BIT;
	gdt_[GDT_CODE64].base_h = 0;
	
	// 64Bit Data
	gdt_[GDT_DATA64].limit_l = 0;
	gdt_[GDT_DATA64].base_l = 0;
	gdt_[GDT_DATA64].base_m = 0;
	gdt_[GDT_DATA64].access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_DATA_CODE |
		GDT_ACCESS_DIRECTION_UP | GDT_ACCESS_RW;
	gdt_[GDT_DATA64].limit_h = 0;
	gdt_[GDT_DATA64].flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_64BIT;
	gdt_[GDT_DATA64].base_h = 0;	
	
	// TSS Entry
	((GDTSystemEntry*)&(gdt_[GDT_TSS]))->limit_l = sizeof(tss_) & 0xFFFF;
	((GDTSystemEntry*)&(gdt_[GDT_TSS]))->base_l = reinterpret_cast<uint64_t>(&tss_) & 0xFFFF;
	((GDTSystemEntry*)&(gdt_[GDT_TSS]))->base_m = (reinterpret_cast<uint64_t>(&tss_) >> 16) & 0xFF;
	((GDTSystemEntry*)&(gdt_[GDT_TSS]))->access = GDT_ACCESS_PRESENT | GDT_ACCESS_DPL0 | GDT_ACCESS_TSS_SEGMENT | GDT_ACCESS_TSS_TYPE64;
	((GDTSystemEntry*)&(gdt_[GDT_TSS]))->limit_h = sizeof(tss_) >> 16 & 0xF;
	((GDTSystemEntry*)&(gdt_[GDT_TSS]))->flags = GDT_FLAGS_GRANULARITY | GDT_FLAGS_64BIT;
	((GDTSystemEntry*)&(gdt_[GDT_TSS]))->base_h = (reinterpret_cast<uint64_t>(&tss_) >> 24) & 0xFF;
	((GDTSystemEntry*)&(gdt_[GDT_TSS]))->base_vh = (reinterpret_cast<uint64_t>(&tss_) >> 32) & 0xFFFFFFFF;
	
	// Setup TSS
	tss_.rsp0 = 0;
	tss_.rsp1 = 0;
	tss_.rsp2 = 0;
	tss_.ist1 = 0;
	tss_.ist2 = 0;
	tss_.ist3 = 0;
	tss_.ist4 = 0;
	tss_.ist5 = 0;
	tss_.ist6 = 0;
	tss_.ist7 = 0;
	tss_.iopb_offset = 104;
	
	
	// GDTD
	gdtd_.base = reinterpret_cast<uint64_t>(gdt_);
	gdtd_.limit = (GDT_TOTAL_COUNT * GDT_ENTRY_SIZE) - 1;
	
	LoadGlobalDescriptorTable();
	LoadTaskStateSegment();
	
	return ReturnValueOk; 
}

void GDT::LoadGlobalDescriptorTable(void) {
	
	asm volatile (
		//"xchgw %%bx, %%bx;\n" //Magic breakpoint in bochs for debugging
		"lgdt %0;\n"
		"movq %%rsp, %%rbx;\n"		//Save RSP
		"movabsq %2, %%rax;\n" 			
		"pushq %%rax;\n"			//Return SS
		"pushq %%rbx;\n" 			//Restore Return RSP
		"movabsq %1, %%rax;\n"
		"pushq %%rax;\n"			//Return CS
		"leaq _ReloadCS(%%rip), %%rax;\n"
		"pushq %%rax;\n"			//Return RIP
		"lretq\n"
		"_ReloadCS:;\n"
		"movabsq %2, %%rbx;\n"
		"mov %%bx, %%ds;\n"
		"mov %%bx, %%es;\n"
		"mov %%bx, %%fs;\n"
		"mov %%bx, %%gs;\n"
		"mov %%bx, %%ss;\n"
		:  
		: "m" (gdtd_), "i" (GDT_CODE64_SEL), "i" (GDT_DATA64_SEL)
		: "rbx", "rax"
	);
}

void GDT::LoadTaskStateSegment(void) {
	
		asm volatile (
		//"xchgw %%bx, %%bx;\n" //Magic breakpoint in bochs for debugging
		"mov %0, %%ax;\n"
		"ltr %%ax;\n"
		:  
		: "i" (GDT_TSS_SEL)
		: "ax"
	);
}

TSS *GDT::GetTaskStateSegment(void) {
	return &tss_;
}

uint16_t GDT::GetSelector(uint64_t selector) {
	return selector;
}

