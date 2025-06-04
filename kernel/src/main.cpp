

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <liminestub.h>
#include <cstub.h>

#include <retval.h>
#include <video/console.h>
#include <cpu/gdt.h>
#include <cpu/interrupt.h>
#include <memory/paging.h>
#include <cpu/pic.h>
#include <cpu/mmio.h>
#include <serial.h>


// For quick testing - needs to be put in a string.h or something similar
char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);
char *htoa(uint64_t num, char* str);


extern "C" uint64_t kmain(void) __attribute__(( nothrow ));
ReturnValue_t InitStage0(void) __attribute__(( nothrow ));
ReturnValue_t InitStage1(void) __attribute__(( nothrow ));
ReturnValue_t InitStage2(void) __attribute__(( nothrow ));
ReturnValue_t InitStage3(void) __attribute__(( nothrow ));

CSerial gSerial;

// For Testing global Constructors
class CGlobalCTORTest {
private:
	volatile uint32_t mTest;

public:
	CGlobalCTORTest() {
		mTest = 5;
	};
	~CGlobalCTORTest() {
		mTest = 2;
	};
	
	uint32_t GetTest(void) {
		return mTest;
	};
	
	void PrintTest(void) {
		char _TempText[24];
		CConsole::Print(itoa(mTest, _TempText, 10));
	};
	
};

CGlobalCTORTest gCTORTest;

// For Testing CPaging::MapAddress()
volatile uint8_t gPagingMapTest[4096] __attribute__ (( aligned(4096) ));

//////

extern "C" uint64_t kmain(void) {

#ifdef _DEBUG
	char _TempText[24];
	const char *_ConstTempText = NULL;
#endif

	ReturnValue_t _RetVal = RETVAL_OK;
	
	
	// First check for correct limine protocol.
	// Everything builds up from this
	_RetVal = CLimine::Init();
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}
		
	// Init Stages
	_RetVal = InitStage0();
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}

	_RetVal = InitStage1();
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}
	
	_RetVal = InitStage2();
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}

	_RetVal = InitStage3();
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}

	
#ifdef _DEBUG
	//Debug Output
	
	// Print HHDM offset
	CConsole::Print("HHDM offset=0x");
	CConsole::Print(htoa((uint64_t)CPaging::GetHHDMOffset(), _TempText));
	CConsole::Print("\n");
	
	
	// Print Framebuffer Address
	CConsole::Print("Framebuffer Address=0x");
	CConsole::Print(htoa((uint64_t)CConsole::GetFramebufferAddress(), _TempText));
	CConsole::Print("\n");
	
	// Print CR3 Address
	CConsole::Print("CR3=0x");
	CConsole::Print(htoa((uint64_t)CPaging::GetCR3(), _TempText));
	CConsole::Print("\n");
	
	
	// Print TSS Address
	CConsole::Print("TSS=0x");
	CConsole::Print(htoa((uint64_t)CGDT::GetTSS(), _TempText));
	CConsole::Print("\n");
#endif
	
#ifdef _DEBUG
	// Interrupt Test
	//asm volatile ("int $0x20;\n");
	// Exception Test
	//asm volatile ("int $0x06;\n");
#endif

#ifdef _DEBUG
	// Test GetPhysicalAddress
	void *_TestVirtualAddress = CConsole::GetFramebufferAddress();
	_TestVirtualAddress = (void*)((uintptr_t)_TestVirtualAddress + 0x1234);
	void *_TestPhysicalAddress = NULL;
	CConsole::Print("Virtual 0x");
	CConsole::Print(htoa((uint64_t)_TestVirtualAddress, _TempText));
	_RetVal = CPaging::GetPhysicalAddress(_TestVirtualAddress, _TestPhysicalAddress);
	if (IS_ERROR(_RetVal)) {
		CConsole::Print(GetReturnValueString(_RetVal));
		CConsole::Print("!\n");
	} else {
		CConsole::Print(" == Physical 0x");
		CConsole::Print(htoa((uint64_t)_TestPhysicalAddress, _TempText));
		CConsole::Print("\n");
	}
#endif	
	
#ifdef _DEBUG
	// Test global CTORs
	CConsole::Print("Global CTOR test=");
	CConsole::Print(itoa(gCTORTest.GetTest(), _TempText, 10));
	CConsole::Print("\n");

	CConsole::Print("Global CTOR test=");
	gCTORTest.PrintTest();
	CConsole::Print("\n");
#endif
	
#ifdef _DEBUG
	// Test CPaging::GetPageLevel
	
	void *_PageLevelTestVirtualAddress = (void*)&gCTORTest;
	_ConstTempText = CPaging::GetPageLevelString(_PageLevelTestVirtualAddress);
	CConsole::Print("Virtual Address 0x");
	CConsole::Print(htoa((uint64_t)_PageLevelTestVirtualAddress, _TempText));
	CConsole::Print(" has page level=");
	CConsole::Print(_ConstTempText);
	CConsole::Print("\n");

	_PageLevelTestVirtualAddress = CPaging::GetHHDMOffset();
	_ConstTempText = CPaging::GetPageLevelString(_PageLevelTestVirtualAddress);
	CConsole::Print("Virtual Address 0x");
	CConsole::Print(htoa((uint64_t)_PageLevelTestVirtualAddress, _TempText));
	CConsole::Print(" has page level=");
	CConsole::Print(_ConstTempText);
	CConsole::Print("\n");
#endif	
	
#ifdef _DEBUG
	//Test CPaging::MapAddress
	CConsole::Print("Test: Mapping gPagingMapTest(");
	CConsole::Print(htoa((uint64_t)&gPagingMapTest, _TempText));
	CConsole::Print(") to 0x7000...\n");
	void *_PagingMapTestPhysicalAddress = (void*)0x7000;
	PageLevel_t _PageLevel = PAGELEVEL_UNKNOWN;
	_RetVal = CPaging::GetPageLevel((void*)&gPagingMapTest, _PageLevel);
	if (IS_ERROR(_RetVal) || (_PageLevel != PAGELEVEL_PML1)) {
		CConsole::Print("ERROR: gPagingMapTest not mapped within PML1!\n");	
	} else {
		_RetVal = CPaging::MapAddress((void*)&gPagingMapTest, _PagingMapTestPhysicalAddress, PAGELEVEL_PML1, CACHETYPE_WRITEBACK, 
			true, false, false);
		if (IS_ERROR(_RetVal)) {
			CConsole::Print(GetReturnValueString(_RetVal));
			CConsole::Print("!\n");			
			return _RetVal;
		}
		
		
		// Check if mapping worked
		_RetVal = CPaging::GetPhysicalAddress((void*)&gPagingMapTest, _PagingMapTestPhysicalAddress);
		if (IS_ERROR(_RetVal)) {
			CConsole::Print(GetReturnValueString(_RetVal));
			CConsole::Print("!\n");
		} else {
			CConsole::Print("Virtual 0x");
			CConsole::Print(htoa((uint64_t)&gPagingMapTest, _TempText));
			CConsole::Print(" == Physical 0x");
			CConsole::Print(htoa((uint64_t)_PagingMapTestPhysicalAddress, _TempText));
			CConsole::Print("\n");
		} 
	}
	
	// Test UnmapAddress
	CConsole::Print("Test: Unmapping gPagingMapTest(");
	CConsole::Print(htoa((uint64_t)&gPagingMapTest, _TempText));
	CConsole::Print(") to 0x7000...\n");
	_RetVal = CPaging::UnmapAddress((void*)&gPagingMapTest, PAGELEVEL_PML1);
	if (IS_ERROR(_RetVal)) {
		CConsole::Print("ERROR: UnmapAddress failed!\n");
		CConsole::Print(GetReturnValueString(_RetVal));
		CConsole::Print("!\n");			
		return _RetVal;
	} else {
		CConsole::Print("UnmapAddress successful!\n");
		
		/* CConsole::Print("Testing Access to unmapped page (should result in a #PF)...\n");
		gPagingMapTest[0] = 5;
		CConsole::Print("If you see this, then UnmapAddress() has a bug!\n"); */
	}
	
#endif	
	
	//Test MMIO
	volatile uint8_t _MMIOTest = mmio_inb(reinterpret_cast<void*>(CConsole::GetFramebufferAddress()));
	mmio_outb(reinterpret_cast<void*>(CConsole::GetFramebufferAddress()), _MMIOTest);
	
	// Test Serial Port
	CConsole::Print("Test: Serial Port 0x3F8...");
	_RetVal = gSerial.Init(0x3F8, 9600, 8, SERIAL_STOPSIZE_1, SERIAL_PARITY_NONE);
	if (IS_ERROR(_RetVal)) {
		CConsole::Print("ERROR!\n");
		return _RetVal;
	}
	CConsole::Print("...OK!\n");
	
	gSerial.Send("HeppOS - Serial test!\n");
	
	CConsole::Print("Done!\n");	
	return RETVAL_OK;
}

//////

ReturnValue_t InitStage0(void) {
	
	//char _TempText[24];
	ReturnValue_t _RetVal = RETVAL_OK;
	
	_RetVal = CPaging::PreInit();
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}
	
	
	return RETVAL_OK;
	
}

//////

ReturnValue_t InitStage1(void) {

	char _TempText[24];
	ReturnValue_t _RetVal = RETVAL_OK;
	
	_RetVal = CConsole::Init(CLimine::GetFramebufferResponse());
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}
	
	CConsole::SetFGColor(0x00AAAAAA);
	CConsole::SetBGColor(0x00000000);
	CConsole::SetTitleFGColor(0x0000FFFF);
	CConsole::SetTitleBGColor(0x000000AA);
	CConsole::SetTitleText("HeppOS");
	CConsole::Clear();
	
	// Print Video Mode
	CConsole::Print("Video Format: ");
	CConsole::Print(utoa(CConsole::GetWidth(), _TempText, 10));
	CConsole::Print("x");
	CConsole::Print(utoa(CConsole::GetHeight(), _TempText, 10));
	CConsole::Print("x");
	CConsole::Print(utoa(CConsole::GetBPP(), _TempText, 10));
	CConsole::Print(" (Pitch=");
	CConsole::Print(utoa(CConsole::GetPitch(), _TempText, 10));
	CConsole::Print(")\n");
	
	return RETVAL_OK;
	
}

//////

ReturnValue_t InitStage2(void) {
	
	//char _TempText[24];
	ReturnValue_t _RetVal = RETVAL_OK;
	
	CConsole::Print("Initializing GDT.........................");
	_RetVal = CGDT::Init();
	if (IS_ERROR(_RetVal)) {
		CConsole::Print(GetReturnValueString(_RetVal));
		CConsole::Print("!\n");
		return _RetVal;
	}
	CConsole::Print("...OK!\n");
	
	return RETVAL_OK;
}

//////

ReturnValue_t InitStage3(void) {
	
	//char _TempText[24];
	ReturnValue_t _RetVal = RETVAL_OK;
	
	CConsole::Print("Initializing IDT.........................");
	_RetVal = CInterrupt::Init();
	if (IS_ERROR(_RetVal)) {
		CConsole::Print(GetReturnValueString(_RetVal));
		CConsole::Print("!\n");
		return _RetVal;
	}
	CConsole::Print("...OK!\n");	
	
	return RETVAL_OK;
}

//////

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
