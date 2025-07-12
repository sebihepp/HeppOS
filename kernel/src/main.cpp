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
	
	
#include <stddef.h>
#include <stdint.h>

#include <liminestub.h>

#include <retval.h>
#include <cpu/gdt.h>
#include <cpu/interrupt.h>
#include <memory/paging.h>
#include <memory/pmm.h>
#include <cpu/pic.h>
#include <cpu/mmio.h>
#include <log.h>
#include <kstring.h>


extern "C" uint64_t kmain(void) __attribute__(( nothrow ));

// For Testing CPaging::MapAddress()
volatile uint8_t gPagingMapTest[4096] __attribute__ (( aligned(4096) ));

//////

extern "C" uint64_t kmain(void) {

#ifdef _DEBUG
	const char *_ConstTempText = NULL;
#endif

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

#ifdef _DEBUG
	CPMM::PrintMemoryMap();
	
	void *_TestAllocAddress = NULL;
	CPMM::Alloc(&_TestAllocAddress, 4096);
	CPMM::PrintMemoryMap();
	
	CLog::PrintF("Allocated Address: %p\n", _TestAllocAddress);

	CPMM::Free(_TestAllocAddress, 4096);
	CPMM::PrintMemoryMap();
	
#endif
	
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
	CLog::PrintF("HHDM offset=%p\n", CPaging::GetHHDMOffset());	
	
	// Print CR3 Address
	CLog::PrintF("CR3=%p\n", CPaging::GetCR3());	
	
	// Print TSS Address
	CLog::PrintF("TSS=%p\n", CGDT::GetTSS());
#endif
	
#ifdef _DEBUG
	// Test GetPhysicalAddress
	void *_TestVirtualAddress = (void*)gPagingMapTest;
	_TestVirtualAddress = (void*)((uintptr_t)_TestVirtualAddress + 0x1234);
	void *_TestPhysicalAddress = NULL;
	CLog::PrintF("Virtual %p", _TestVirtualAddress);
	_RetVal = CPaging::GetPhysicalAddress(_TestVirtualAddress, _TestPhysicalAddress);
	if (IS_ERROR(_RetVal)) {
		CLog::PrintF("%s!\n", GetReturnValueString(_RetVal));
	} else {
		CLog::PrintF(" == Physical %p\n", _TestPhysicalAddress);
	}
#endif	
	
#ifdef _DEBUG
	// Test CPaging::GetPageLevel
	
	void *_PageLevelTestVirtualAddress = (void*)gPagingMapTest;
	_ConstTempText = CPaging::GetPageLevelString(_PageLevelTestVirtualAddress);
	CLog::PrintF("Virtual Address %p has page level=%s\n", _PageLevelTestVirtualAddress, _ConstTempText);

	_PageLevelTestVirtualAddress = CPaging::GetHHDMOffset();
	_ConstTempText = CPaging::GetPageLevelString(_PageLevelTestVirtualAddress);
	CLog::PrintF("Virtual Address %p has page level=%s\n", _PageLevelTestVirtualAddress, _ConstTempText);
#endif	
	
#ifdef _DEBUG
	//Test CPaging::MapAddress
	CLog::PrintF("Test: Mapping gPagingMapTest(%p) to 0x7000...\n", &gPagingMapTest);
	void *_PagingMapTestPhysicalAddress = (void*)0x7000;
	PageLevel_t _PageLevel = PAGELEVEL_UNKNOWN;
	_RetVal = CPaging::GetPageLevel((void*)&gPagingMapTest, _PageLevel);
	if (IS_ERROR(_RetVal) || (_PageLevel != PAGELEVEL_PML1)) {
		CLog::Print("ERROR: gPagingMapTest not mapped within PML1!\n");	
	} else {
		_RetVal = CPaging::MapAddress((void*)&gPagingMapTest, _PagingMapTestPhysicalAddress, PAGELEVEL_PML1, CACHETYPE_WRITEBACK, 
			true, false, false);
		if (IS_ERROR(_RetVal)) {
			CLog::PrintF("%s!\n", GetReturnValueString(_RetVal));		
			return _RetVal;
		}
		
		
		// Check if mapping worked
		_RetVal = CPaging::GetPhysicalAddress((void*)&gPagingMapTest, _PagingMapTestPhysicalAddress);
		if (IS_ERROR(_RetVal)) {
			CLog::PrintF("%s!\n", GetReturnValueString(_RetVal));
		} else {
			CLog::PrintF("Virtual %p == Physical %p\n", &gPagingMapTest, _PagingMapTestPhysicalAddress);
		} 
	}
	
	// Test UnmapAddress
	CLog::PrintF("Test: Unmapping gPagingMapTest(%p) to 0x7000...\n", &gPagingMapTest);
	_RetVal = CPaging::UnmapAddress((void*)&gPagingMapTest, PAGELEVEL_PML1);
	if (IS_ERROR(_RetVal)) {
		CLog::Print("ERROR: UnmapAddress failed!\n");
		CLog::PrintF("%s!\n", GetReturnValueString(_RetVal));
		return _RetVal;
	} else {
		CLog::Print("UnmapAddress successful!\n");
		
		/* CLog::Print("Testing Access to unmapped page (should result in a #PF)...\n");
		gPagingMapTest[0] = 5;
		CLog::Print("If you see this, then UnmapAddress() has a bug!\n"); */
	}
	
#endif	
		
#ifdef _DEBUG	
	// Test kstring
	
	const char *_KStringTest = "Hello World!";
	CLog::PrintF("Size of \"%s\" is %d!\n", _KStringTest, kstrlen(_KStringTest));
	
	char _KStringTest2[64];
	kstrncpy(_KStringTest2, "Hello ", 64);
	CLog::PrintF("Output of strcpy and strcat: %s\n", kstrcat(_KStringTest2, "World!"));
	
	CLog::PrintF("Comparing the two strings = %d!\n", kstrcmp(_KStringTest, _KStringTest2));
	
	_KStringTest2[0] = 'h';
	CLog::PrintF("Comparing \"%s\" with \"%s\" = %d!\n", _KStringTest, _KStringTest2, kstricmp(_KStringTest, _KStringTest2));

	const char *_KStringTest3 = kstrpbrk(_KStringTest2, " !");
	CLog::PrintF("kstrpbrk = %s\n", _KStringTest3);
	
	const char *_KStringTest4 = _KStringTest2 + kstrspn(_KStringTest2, "abcdefghijklmnopqrstuvwxyz");
	CLog::PrintF("kstrspn = %s\n", _KStringTest4);

	const char *_KStringTest5 = _KStringTest2 + kstrcspn(_KStringTest2, "abcdefghijklmnopqrstuvwxyz");
	CLog::PrintF("kstrcspn = %s\n", _KStringTest5);
	
	CLog::PrintF("kstrrev = %s\n", kstrrev(_KStringTest2));

	CLog::PrintF("kstrstr = %s\n", kstrstr(_KStringTest, "rl"));

	
	CLog::PrintF("%%c = %c\n", 'a');
	CLog::PrintF("%%s = %s\n", "Hallo 123...");
	CLog::PrintF("%%s = %s\n", "Another Test");
	CLog::PrintF("%%d = %d\n", -15);
	CLog::PrintF("%%o = %o\n", 12);
	CLog::PrintF("%%X = %X\n", 0xAA55AA55);
	CLog::PrintF("%%p = %p\n", (void*)_KStringTest2);
	
	CLog::PrintF("Width Test: 0x%4x!\n", 7);
	CLog::PrintF("Leading Zero Test: 0x%04x!\n", 7);
	CLog::PrintF("Leading Zero Test: 0o%04o!\n", -9);
	CLog::PrintF("Leading Zero Test: %04d!\n", -9);
	
	CLog::PrintF("Last kstring test:\nkstrlen(_KStringTest2 <%p>)=%d\n\n", (void*)_KStringTest2, kstrlen(_KStringTest2));
	
	asm volatile ("int $0x06;\n");
		
#endif


	CLog::Print("Done!\n");
	return RETVAL_OK;
}

