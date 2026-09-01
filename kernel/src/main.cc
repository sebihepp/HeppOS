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
	
	
#include <ktype.h>
#include <return_value.h>

#include <limine_stub.h>

#include <cpu/gdt.h>
#include <cpu/interrupt.h>
#include <memory/paging.h>
#include <memory/pmm.h>
#include <cpu/mmio.h>
#include <boot_log.h>
#include <kstring.h>


extern "C" uint64_t kmain(uint64_t rdi) __attribute__((nothrow));

ReturnValue KInit(void) __attribute__((nothrow));



extern "C" uint64_t kmain(uint64_t rdi) {

	ReturnValue return_value = ReturnValueOk;

	if (rdi == 0) {
		return_value = KInit();
		if (IS_ERROR(return_value)) {
			BootLog::Print(GetReturnValueString(return_value));
			return ReturnValueOk;
		}
	}




	return ReturnValueOk;
}

ReturnValue KInit(void) {

	ReturnValue return_value = ReturnValueOk;

	// Initialize Logging
	BootLog::Init();
	BootLog::Print("\n\nHeppOS\n");
	
	// First check for correct limine protocol.
	// Everything builds up from this
	BootLog::Print("Init CLimine...");
	return_value = LimineStub::Init();
	BootLog::Print(GetReturnValueString(return_value));
	BootLog::Print("\n");
	if (IS_ERROR(return_value)) {
		return return_value;
	}
	
	// PreInitialize Paging
	BootLog::Print("PreInit CPaging...");
	return_value = Paging::PreInit();
	BootLog::Print(GetReturnValueString(return_value));
	BootLog::Print("\n");
	if (IS_ERROR(return_value)) {
		return return_value;
	}
	
#ifdef _DEBUG

	// Print Kernel Physical base
	BootLog::PrintF(
	    "Kernel physical base=%016p\n",
	    LimineStub::GetExecutableAddressResponse()->physical_base);	

	// Print Kernel Virtual base
	BootLog::PrintF(
	    "Kernel virtual base=%016p\n",
	    LimineStub::GetExecutableAddressResponse()->virtual_base);	

	// Print HHDM offset
	BootLog::PrintF("HHDM offset=%016p\n", Paging::GetHHDMOffset());	

#endif 


	// PreInitialize PhysicalMemoryManager
	BootLog::Print("PreInit CPMM...");
	return_value = CPMM::PreInit();
	BootLog::Print(GetReturnValueString(return_value));
	BootLog::Print("\n");
	if (IS_ERROR(return_value)) {
		return return_value;
	}

	// Print Used Memory
	const char *memory_unit = "B";
	uint64_t used_memory = CPMM::GetUsedMemory();
	if (used_memory > 100000) {
		used_memory /= 1024;
		memory_unit = "KiB";
	}
	if (used_memory > 100000) {
		used_memory /= 1024;
		memory_unit = "MiB";
	}
	if (used_memory > 100000) {
		used_memory /= 1024;
		memory_unit = "GiB";
	}
	BootLog::PrintF("Used Memory: %ld%s\n", used_memory, memory_unit);
	
	// Print Free Memory
	memory_unit = "B";
	uint64_t free_memory = CPMM::GetFreeMemory();
	if (free_memory > 100000) {
		free_memory /= 1024;
		memory_unit = "KiB";
	}
	if (free_memory > 100000) {
		free_memory /= 1024;
		memory_unit = "MiB";
	}
	if (free_memory > 100000) {
		free_memory /= 1024;
		memory_unit = "GiB";
	}
	BootLog::PrintF("Free Memory: %ld%s\n", free_memory, memory_unit);
	
	// Initialize GDT
	BootLog::Print("Init CGDT...");
	return_value = GDT::Init();
	BootLog::Print(GetReturnValueString(return_value));
	BootLog::Print("\n");
	if (IS_ERROR(return_value)) {
		return return_value;
	}

	// PreInitialize IDT and Interrupts
	BootLog::Print("Init CInterrupt...");
	return_value = Interrupt::Init();
	BootLog::Print(GetReturnValueString(return_value));
	BootLog::Print("\n");
	if (IS_ERROR(return_value)) {
		return return_value;
	}
	
#ifdef _DEBUG
	//Debug Output

	// Print Framebuffer
	BootLog::PrintF("Framebuffer-Format: %dx%dx%d\nFramebuffer-Address=%016p\n", 
		LimineStub::GetFramebufferResponse()->framebuffers[0]->width, 
		LimineStub::GetFramebufferResponse()->framebuffers[0]->height, 
		LimineStub::GetFramebufferResponse()->framebuffers[0]->bpp,
		LimineStub::GetFramebufferResponse()->framebuffers[0]->address);

	// Print CR3 Address
	BootLog::PrintF("CR3=%016p\n", Paging::GetCR3());	
	
	// Print TSS Address
	BootLog::PrintF("TSS=%016p\n", GDT::GetTaskStateSegment());
#endif

	BootLog::Print("Done!\n");
	return ReturnValueOk;
}
