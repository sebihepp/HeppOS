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

	// Print Used Memory
	const char *_MemoryUnit = "B";
	uint64_t _UsedMemory = CPMM::GetUsedMemory();
	if (_UsedMemory > 4096) {
		_UsedMemory /= 1024;
		_MemoryUnit = "KB";
	}
	if (_UsedMemory > 4096) {
		_UsedMemory /= 1024;
		_MemoryUnit = "MB";
	}
	if (_UsedMemory > 4096) {
		_UsedMemory /= 1024;
		_MemoryUnit = "GB";
	}
	CLog::PrintF("Used Memory: %d%s\n", _UsedMemory, _MemoryUnit);
	
	// Print Free Memory
	_MemoryUnit = "B";
	uint64_t _FreeMemory = CPMM::GetFreeMemory();
	if (_FreeMemory > 4096) {
		_FreeMemory /= 1024;
		_MemoryUnit = "KB";
	}
	if (_FreeMemory > 4096) {
		_FreeMemory /= 1024;
		_MemoryUnit = "MB";
	}
	if (_FreeMemory > 4096) {
		_FreeMemory /= 1024;
		_MemoryUnit = "GB";
	}
	CLog::PrintF("Free Memory: %d%s\n", _FreeMemory, _MemoryUnit);


#ifdef _DEBUG

	//Testing CPMM
	
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
	CLog::PrintF("HHDM offset=%016p\n", CPaging::GetHHDMOffset());	
	
	// Print CR3 Address
	CLog::PrintF("CR3=%016p\n", CPaging::GetCR3());	
	
	// Print TSS Address
	CLog::PrintF("TSS=%016p\n", CGDT::GetTSS());
#endif
	
#ifdef _DEBUG
	// Test GetPhysicalAddress
	VirtualAddress_t _TestVirtualAddress = (VirtualAddress_t)&gPagingMapTest;
	_TestVirtualAddress = _TestVirtualAddress + 0x1234;
	PhysicalAddress_t _TestPhysicalAddress = (PhysicalAddress_t)NULL;
	CLog::PrintF("Virtual %016p", _TestVirtualAddress);
	_RetVal = CPaging::GetPhysicalAddress(_TestVirtualAddress, _TestPhysicalAddress);
	if (IS_ERROR(_RetVal)) {
		CLog::PrintF("%s!\n", GetReturnValueString(_RetVal));
	} else {
		CLog::PrintF(" == Physical %016p\n", _TestPhysicalAddress);
	}
#endif	
	
#ifdef _DEBUG
	// Test CPaging::GetPageLevel
	
	VirtualAddress_t _PageLevelTestVirtualAddress = (VirtualAddress_t)gPagingMapTest;
	_ConstTempText = CPaging::GetPageLevelString(_PageLevelTestVirtualAddress);
	CLog::PrintF("Virtual Address %016p has page level=%s\n", _PageLevelTestVirtualAddress, _ConstTempText);

	_PageLevelTestVirtualAddress = CPaging::GetHHDMOffset();
	_ConstTempText = CPaging::GetPageLevelString(_PageLevelTestVirtualAddress);
	CLog::PrintF("Virtual Address %016p has page level=%s\n", _PageLevelTestVirtualAddress, _ConstTempText);
#endif	
	
#ifdef _DEBUG
	//Test CPaging::MapAddress
	CLog::PrintF("Test: Mapping gPagingMapTest(%016p) to 0x7000...\n", &gPagingMapTest);
	PhysicalAddress_t _PagingMapTestPhysicalAddress = (PhysicalAddress_t)0x7000;
	PageLevel_t _PageLevel = PAGELEVEL_UNKNOWN;
	_RetVal = CPaging::GetPageLevel((VirtualAddress_t)&gPagingMapTest, _PageLevel);
	if (IS_ERROR(_RetVal) || (_PageLevel != PAGELEVEL_PML1)) {
		CLog::Print("ERROR: gPagingMapTest not mapped within PML1!\n");	
	} else {
		_RetVal = CPaging::MapAddress((VirtualAddress_t)&gPagingMapTest, _PagingMapTestPhysicalAddress, PAGELEVEL_PML1, CACHETYPE_WRITEBACK, 
			true, false, false);
		if (IS_ERROR(_RetVal)) {
			CLog::PrintF("%s!\n", GetReturnValueString(_RetVal));		
			return _RetVal;
		}
		
		
		// Check if mapping worked
		_RetVal = CPaging::GetPhysicalAddress((VirtualAddress_t)&gPagingMapTest, _PagingMapTestPhysicalAddress);
		if (IS_ERROR(_RetVal)) {
			CLog::PrintF("%s!\n", GetReturnValueString(_RetVal));
		} else {
			CLog::PrintF("Virtual %016p == Physical %016p\n", &gPagingMapTest, _PagingMapTestPhysicalAddress);
		} 
	}
	
	// Test UnmapAddress
	CLog::PrintF("Test: Unmapping gPagingMapTest(%016p) to 0x7000...\n", &gPagingMapTest);
	_RetVal = CPaging::UnmapAddress((VirtualAddress_t)&gPagingMapTest, PAGELEVEL_PML1);
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

