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
	char _TempText[24];
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
	CLog::Print("HHDM offset=0x");
	CLog::Print(kitoa((uint64_t)CPaging::GetHHDMOffset(), _TempText, 16));
	CLog::Print("\n");
	
	
	// Print Framebuffer Address
	/* CLog::Print("Framebuffer Address=0x");
	CLog::Print(kitoa((uint64_t)CConsole::GetFramebufferAddress(), _TempText, 16));
	CLog::Print("\n"); */
	
	// Print CR3 Address
	CLog::Print("CR3=0x");
	CLog::Print(kitoa((uint64_t)CPaging::GetCR3(), _TempText, 16));
	CLog::Print("\n");
	
	
	// Print TSS Address
	CLog::Print("TSS=0x");
	CLog::Print(kitoa((uint64_t)CGDT::GetTSS(), _TempText, 16));
	CLog::Print("\n");
#endif
	
#ifdef _DEBUG
	// Test GetPhysicalAddress
	void *_TestVirtualAddress = (void*)gPagingMapTest;
	_TestVirtualAddress = (void*)((uintptr_t)_TestVirtualAddress + 0x1234);
	void *_TestPhysicalAddress = NULL;
	CLog::Print("Virtual 0x");
	CLog::Print(kitoa((uint64_t)_TestVirtualAddress, _TempText, 16));
	_RetVal = CPaging::GetPhysicalAddress(_TestVirtualAddress, _TestPhysicalAddress);
	if (IS_ERROR(_RetVal)) {
		CLog::Print(GetReturnValueString(_RetVal));
		CLog::Print("!\n");
	} else {
		CLog::Print(" == Physical 0x");
		CLog::Print(kitoa((uint64_t)_TestPhysicalAddress, _TempText, 16));
		CLog::Print("\n");
	}
#endif	
	
#ifdef _DEBUG
	// Test CPaging::GetPageLevel
	
	void *_PageLevelTestVirtualAddress = (void*)gPagingMapTest;
	_ConstTempText = CPaging::GetPageLevelString(_PageLevelTestVirtualAddress);
	CLog::Print("Virtual Address 0x");
	CLog::Print(kitoa((uint64_t)_PageLevelTestVirtualAddress, _TempText, 16));
	CLog::Print(" has page level=");
	CLog::Print(_ConstTempText);
	CLog::Print("\n");

	_PageLevelTestVirtualAddress = CPaging::GetHHDMOffset();
	_ConstTempText = CPaging::GetPageLevelString(_PageLevelTestVirtualAddress);
	CLog::Print("Virtual Address 0x");
	CLog::Print(kitoa((uint64_t)_PageLevelTestVirtualAddress, _TempText, 16));
	CLog::Print(" has page level=");
	CLog::Print(_ConstTempText);
	CLog::Print("\n");
#endif	
	
#ifdef _DEBUG
	//Test CPaging::MapAddress
	CLog::Print("Test: Mapping gPagingMapTest(");
	CLog::Print(kitoa((uint64_t)&gPagingMapTest, _TempText, 16));
	CLog::Print(") to 0x7000...\n");
	void *_PagingMapTestPhysicalAddress = (void*)0x7000;
	PageLevel_t _PageLevel = PAGELEVEL_UNKNOWN;
	_RetVal = CPaging::GetPageLevel((void*)&gPagingMapTest, _PageLevel);
	if (IS_ERROR(_RetVal) || (_PageLevel != PAGELEVEL_PML1)) {
		CLog::Print("ERROR: gPagingMapTest not mapped within PML1!\n");	
	} else {
		_RetVal = CPaging::MapAddress((void*)&gPagingMapTest, _PagingMapTestPhysicalAddress, PAGELEVEL_PML1, CACHETYPE_WRITEBACK, 
			true, false, false);
		if (IS_ERROR(_RetVal)) {
			CLog::Print(GetReturnValueString(_RetVal));
			CLog::Print("!\n");			
			return _RetVal;
		}
		
		
		// Check if mapping worked
		_RetVal = CPaging::GetPhysicalAddress((void*)&gPagingMapTest, _PagingMapTestPhysicalAddress);
		if (IS_ERROR(_RetVal)) {
			CLog::Print(GetReturnValueString(_RetVal));
			CLog::Print("!\n");
		} else {
			CLog::Print("Virtual 0x");
			CLog::Print(kitoa((uint64_t)&gPagingMapTest, _TempText, 16));
			CLog::Print(" == Physical 0x");
			CLog::Print(kitoa((uint64_t)_PagingMapTestPhysicalAddress, _TempText, 16));
			CLog::Print("\n");
		} 
	}
	
	// Test UnmapAddress
	CLog::Print("Test: Unmapping gPagingMapTest(");
	CLog::Print(kitoa((uint64_t)&gPagingMapTest, _TempText, 16));
	CLog::Print(") to 0x7000...\n");
	_RetVal = CPaging::UnmapAddress((void*)&gPagingMapTest, PAGELEVEL_PML1);
	if (IS_ERROR(_RetVal)) {
		CLog::Print("ERROR: UnmapAddress failed!\n");
		CLog::Print(GetReturnValueString(_RetVal));
		CLog::Print("!\n");			
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
	CLog::Print("Size of \"");
	CLog::Print(_KStringTest);
	CLog::Print("\" is ");
	CLog::Print(kitoa(kstrlen(_KStringTest), _TempText, 10));
	CLog::Print("!\n");
	
	char _KStringTest2[64];
	CLog::Print("Output of strcpy and strcat: ");
	kstrncpy(_KStringTest2, "Hello ", 64);
	CLog::Print(kstrcat(_KStringTest2, "World!"));
	CLog::Print("\n");
	
	CLog::Print("Comparing the two strings = ");
	CLog::Print(kitoa(kstrcmp(_KStringTest, _KStringTest2), _TempText, 10));
	CLog::Print("!\n");
	
	_KStringTest2[0] = 'h';
	CLog::Print("Comparing \"");
	CLog::Print(_KStringTest);
	CLog::Print("\" with \"");
	CLog::Print(_KStringTest2);
	CLog::Print("\" = ");
	CLog::Print(kitoa(kstricmp(_KStringTest, _KStringTest2), _TempText, 10));
	CLog::Print("!\n");

	const char *_KStringTest3 = kstrpbrk(_KStringTest2, " !");
	CLog::Print("kstrpbrk = ");
	CLog::Print(_KStringTest3);
	CLog::Print("\n");
	
	const char *_KStringTest4 = _KStringTest2 + kstrspn(_KStringTest2, "abcdefghijklmnopqrstuvwxyz");
	CLog::Print("kstrspn = ");
	CLog::Print(_KStringTest4);
	CLog::Print("\n");

	const char *_KStringTest5 = _KStringTest2 + kstrcspn(_KStringTest2, "abcdefghijklmnopqrstuvwxyz");
	CLog::Print("kstrcspn = ");
	CLog::Print(_KStringTest5);
	CLog::Print("\n");
	
	CLog::Print("kstrrev = ");
	CLog::Print(kstrrev(_KStringTest2));
	CLog::Print("\n");

	CLog::Print("kstrstr = ");
	CLog::Print(kstrstr(_KStringTest, "rl"));
	CLog::Print("\n");

	CLog::Print("CLog::PrintF() test:\n");
	
	CLog::PrintF("%%c = %c\n", 'a');
	CLog::PrintF("%%s = %s\n", "Hallo 123...");
	CLog::PrintF("%%s = %s\n", "Another Test");
	CLog::PrintF("%%d = %d\n", -15);
	CLog::PrintF("%%o = %o\n", 12);
	CLog::PrintF("%%X = %X\n", 0xAA55AA55);
	CLog::PrintF("%%p = %p\n", (void*)_KStringTest2);
	
CLog::PrintF("Last kstring test:\nkstrlen(_KStringTest2 <%p>)=%d\n\n", (void*)_KStringTest2, kstrlen(_KStringTest2));
		
#endif


	CLog::Print("Done!\n");
	return RETVAL_OK;
}

