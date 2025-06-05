

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <liminestub.h>
#include <cstub.h>

#include <retval.h>
#include <cpu/gdt.h>
#include <cpu/interrupt.h>
#include <memory/paging.h>
#include <cpu/pic.h>
#include <cpu/mmio.h>
#include <log.h>


// For quick testing - needs to be put in a string.h or something similar
char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);
char *htoa(uint64_t num, char* str);


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
	CLog::Print("PreInit CGDT...");
	_RetVal = CGDT::Init();
	CLog::Print(GetReturnValueString(_RetVal));
	CLog::Print("\n");
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}

	// PreInitialize IDT and Interrupts
	CLog::Print("PreInit CInterrupt...");
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
	CLog::Print(htoa((uint64_t)CPaging::GetHHDMOffset(), _TempText));
	CLog::Print("\n");
	
	
	// Print Framebuffer Address
	/* CLog::Print("Framebuffer Address=0x");
	CLog::Print(htoa((uint64_t)CConsole::GetFramebufferAddress(), _TempText));
	CLog::Print("\n"); */
	
	// Print CR3 Address
	CLog::Print("CR3=0x");
	CLog::Print(htoa((uint64_t)CPaging::GetCR3(), _TempText));
	CLog::Print("\n");
	
	
	// Print TSS Address
	CLog::Print("TSS=0x");
	CLog::Print(htoa((uint64_t)CGDT::GetTSS(), _TempText));
	CLog::Print("\n");
#endif
	
#ifdef _DEBUG
	// Test GetPhysicalAddress
	void *_TestVirtualAddress = (void*)gPagingMapTest;
	_TestVirtualAddress = (void*)((uintptr_t)_TestVirtualAddress + 0x1234);
	void *_TestPhysicalAddress = NULL;
	CLog::Print("Virtual 0x");
	CLog::Print(htoa((uint64_t)_TestVirtualAddress, _TempText));
	_RetVal = CPaging::GetPhysicalAddress(_TestVirtualAddress, _TestPhysicalAddress);
	if (IS_ERROR(_RetVal)) {
		CLog::Print(GetReturnValueString(_RetVal));
		CLog::Print("!\n");
	} else {
		CLog::Print(" == Physical 0x");
		CLog::Print(htoa((uint64_t)_TestPhysicalAddress, _TempText));
		CLog::Print("\n");
	}
#endif	
	
#ifdef _DEBUG
	// Test CPaging::GetPageLevel
	
	void *_PageLevelTestVirtualAddress = (void*)gPagingMapTest;
	_ConstTempText = CPaging::GetPageLevelString(_PageLevelTestVirtualAddress);
	CLog::Print("Virtual Address 0x");
	CLog::Print(htoa((uint64_t)_PageLevelTestVirtualAddress, _TempText));
	CLog::Print(" has page level=");
	CLog::Print(_ConstTempText);
	CLog::Print("\n");

	_PageLevelTestVirtualAddress = CPaging::GetHHDMOffset();
	_ConstTempText = CPaging::GetPageLevelString(_PageLevelTestVirtualAddress);
	CLog::Print("Virtual Address 0x");
	CLog::Print(htoa((uint64_t)_PageLevelTestVirtualAddress, _TempText));
	CLog::Print(" has page level=");
	CLog::Print(_ConstTempText);
	CLog::Print("\n");
#endif	
	
#ifdef _DEBUG
	//Test CPaging::MapAddress
	CLog::Print("Test: Mapping gPagingMapTest(");
	CLog::Print(htoa((uint64_t)&gPagingMapTest, _TempText));
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
			CLog::Print(htoa((uint64_t)&gPagingMapTest, _TempText));
			CLog::Print(" == Physical 0x");
			CLog::Print(htoa((uint64_t)_PagingMapTestPhysicalAddress, _TempText));
			CLog::Print("\n");
		} 
	}
	
	// Test UnmapAddress
	CLog::Print("Test: Unmapping gPagingMapTest(");
	CLog::Print(htoa((uint64_t)&gPagingMapTest, _TempText));
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
		
	CLog::Print("Done!\n");
	return RETVAL_OK;
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char* itoa(int num, char* str, int base)
{
	int i = 0;
	bool isNegative = false;
	
	/* Handle 0 explicitly, otherwise empty string is
	* printed for 0 */
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
	
	// In standard itoa(), negative numbers are handled
	// only with base 10. Otherwise numbers are
	// considered unsigned.
	if (num < 0 && base == 10) {
		isNegative = true;
		num = -num;
	}
	
	// Process individual digits
	while (num != 0) {
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}
	
	// If number is negative, append '-'
	if (isNegative)
		str[i++] = '-';
	
	str[i] = '\0'; // Append string terminator
	
	// Reverse the string
	reverse(str, i);
 
	return str;
}

char *utoa (unsigned value, char *str, int base) {
  const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  int i, j;
  unsigned remainder;
  char c;
  
  /* Check base is supported. */
  if ((base < 2) || (base > 36))
    { 
      str[0] = '\0';
      return NULL;
    }  
    
  /* Convert to string. Digits are in reverse order.  */
  i = 0;
  do 
    {
      remainder = value % base;
      str[i++] = digits[remainder];
      value = value / base;
    } while (value != 0);  
  str[i] = '\0'; 
  
  /* Reverse string.  */
  for (j = 0, i--; j < i; j++, i--)
    {
      c = str[j];
      str[j] = str[i];
      str[i] = c; 
    }       
  
  return str;
}

char *htoa(uint64_t num, char* str) {
	
	const char digits[] = "0123456789abcdef";
	
	for (uint8_t i = 0; i < 16; i++) {
		uint32_t _val = (num >> (60 - (i * 4))) & 0xF;
		str[i] = digits[_val];
	}
	str [16] = '\0';
	return str;
}
