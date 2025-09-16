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

#include <liminestub.h>

#include <cpu/gdt.h>
#include <cpu/interrupt.h>
#include <memory/paging.h>
#include <memory/pmm.h>
#include <cpu/mmio.h>
#include <log.h>
#include <kstring.h>


extern "C" uint64_t kmain(uint64_t pRDI) __attribute__(( nothrow ));

ReturnValue_t KInit(void) __attribute__(( nothrow ));



extern "C" uint64_t kmain(uint64_t pRDI) {

	ReturnValue_t _RetVal = RETVAL_OK;

	if (pRDI == 0) {
		_RetVal = KInit();
		if (IS_ERROR(_RetVal)) {
			CLog::Print(GetReturnValueString(_RetVal));
			return _RetVal;
		}
	}




	return RETVAL_OK;
}

ReturnValue_t KInit(void) {

	ReturnValue_t _RetVal = RETVAL_OK;

	// Initialize Logging
	CLog::Init();
	CLog::Print("\n\nHeppOS\n");
	
	// First check for correct limine protocol.
	// Everything builds up from this
	CLog::Print("Init CLimine...");
	_RetVal = CLimine::Init();
	CLog::Print(GetReturnValueString(_RetVal));
	CLog::Print("\n");
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}
	
	// PreInitialize Paging
	CLog::Print("PreInit CPaging...");
	_RetVal = CPaging::PreInit();
	CLog::Print(GetReturnValueString(_RetVal));
	CLog::Print("\n");
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}
	
	// PreInitialize PhysicalMemoryManager
	CLog::Print("PreInit CPMM...");
	_RetVal = CPMM::PreInit();
	CLog::Print(GetReturnValueString(_RetVal));
	CLog::Print("\n");
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}

	// Print Used Memory
	const char *_MemoryUnit = "B";
	uint64_t _UsedMemory = CPMM::GetUsedMemory();
	if (_UsedMemory > 100000) {
		_UsedMemory /= 1024;
		_MemoryUnit = "KiB";
	}
	if (_UsedMemory > 100000) {
		_UsedMemory /= 1024;
		_MemoryUnit = "MiB";
	}
	if (_UsedMemory > 100000) {
		_UsedMemory /= 1024;
		_MemoryUnit = "GiB";
	}
	CLog::PrintF("Used Memory: %d%s\n", _UsedMemory, _MemoryUnit);
	
	// Print Free Memory
	_MemoryUnit = "B";
	uint64_t _FreeMemory = CPMM::GetFreeMemory();
	if (_FreeMemory > 100000) {
		_FreeMemory /= 1024;
		_MemoryUnit = "KiB";
	}
	if (_FreeMemory > 100000) {
		_FreeMemory /= 1024;
		_MemoryUnit = "MiB";
	}
	if (_FreeMemory > 100000) {
		_FreeMemory /= 1024;
		_MemoryUnit = "GiB";
	}
	CLog::PrintF("Free Memory: %d%s\n", _FreeMemory, _MemoryUnit);
	
	// Initialize GDT
	CLog::Print("Init CGDT...");
	_RetVal = CGDT::Init();
	CLog::Print(GetReturnValueString(_RetVal));
	CLog::Print("\n");
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}

	// PreInitialize IDT and Interrupts
	CLog::Print("Init CInterrupt...");
	_RetVal = CInterrupt::Init();
	CLog::Print(GetReturnValueString(_RetVal));
	CLog::Print("\n");
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}
	
#ifdef _DEBUG
	//Debug Output
	
	// Print HHDM offset
	CLog::PrintF("HHDM offset=%016p\n", CPaging::GetHHDMOffset());	

	// Print Kernel Physical base
	CLog::PrintF("Kernel physical base=%016p\n", CLimine::GetExecutableAddressResponse()->physical_base);	

	// Print Kernel Virtual base
	CLog::PrintF("Kernel virtual base=%016p\n", CLimine::GetExecutableAddressResponse()->virtual_base);	


	// Print Framebuffer
	CLog::PrintF("Framebuffer-Format: %dx%dx%d\nFramebuffer-Address=%016p\n", CLimine::GetFramebufferResponse()->framebuffers[0]->width, 
		CLimine::GetFramebufferResponse()->framebuffers[0]->height, CLimine::GetFramebufferResponse()->framebuffers[0]->bpp,
		CLimine::GetFramebufferResponse()->framebuffers[0]->address);

	// Print CR3 Address
	CLog::PrintF("CR3=%016p\n", CPaging::GetCR3());	
	
	// Print TSS Address
	CLog::PrintF("TSS=%016p\n", CGDT::GetTSS());
#endif

	CLog::Print("Done!\n");
	return RETVAL_OK;
}
