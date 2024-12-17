

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <liminestub.h>
#include <cstub.h>

#include <retvals.h>
#include <video/console.h>
#include <cpu/gdt.h>
#include <cpu/interrupt.h>
#include <memory/paging.h>
#include <cpu/pic.h>


// For quick testing - needs to be put in a string.h or something similar
char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);
char *htoa(uint64_t num, char* str);


extern "C" uint32_t kmain(void) {

	char _TempText[24];
	ReturnValue_t _RetVal = RETVAL_OK;
	
	_RetVal = CLimine::Init();
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}
		
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
	
	//Debug Output
	
	
	// Print HHDM offset
	CConsole::Print("HHDM offset=0x");
	CConsole::Print(htoa(CLimine::GetHHDMResponse()->offset, _TempText));
	CConsole::Print("\n");
	
	
	// Print Framebuffer Address
	CConsole::Print("Framebuffer Address=0x");
	CConsole::Print(htoa((uint64_t)CLimine::GetFramebufferResponse()->framebuffers[0]->address, _TempText));
	CConsole::Print("\n");
	
	// Print CR3 Address
	CConsole::Print("CR3=0x");
	CConsole::Print(htoa((uint64_t)CPaging::GetCR3(), _TempText));
	CConsole::Print("\n");
	
	
	// Print TSS Address
	CConsole::Print("TSS=0x");
	CConsole::Print(htoa((uint64_t)CGDT::GetTSS(), _TempText));
	CConsole::Print("\n");
	
	
	
	CConsole::Print("Initializing GDT.........................");
	_RetVal = CGDT::Init();
	if (IS_ERROR(_RetVal)) {
		CConsole::Print("ERROR!\n");
		return _RetVal;
	}
	CConsole::Print("...OK!\n");
	
	CConsole::Print("Loading GDT..............................");
	CGDT::LoadGDT();
	CConsole::Print("...OK!\n");

	CConsole::Print("Loading TSS..............................");
	CGDT::LoadTSS();
	CConsole::Print("...OK!\n");
	
	CConsole::Print("Initializing IDT.........................");
	_RetVal = CInterrupt::Init();
	if (IS_ERROR(_RetVal)) {
		CConsole::Print("ERROR!\n");
		return _RetVal;
	}
	CConsole::Print("...OK!\n");

	CConsole::Print("Loading IDT..............................");
	CInterrupt::LoadIDT();
	CConsole::Print("...OK!\n");


	// Testing Cariage Return
/* 	CConsole::Print("Testing Carriage Return..................ERROR!");
	CConsole::Print("\rTesting Carriage Return.....................OK!\n"); */
	
	// Testing tabulator
	/* CConsole::Print("Testing Tabulator...\tTEST\tTEST2\tTTT\tOK!\n"); */
	
	
	// Testing Handler
/* 	for (uint64_t i = 0; i < 5; i++) {
		CConsole::Print("Testing Handler 128...\n");
		asm volatile (
			"int $0x80;\n"
		);
	} 
 	CConsole::Print("Testing Handler 39...\n");
	asm volatile (
		"int $0x27;\n"
	);
	 */
	 
	// Testing Page Fault Exception
/* 	CConsole::Print("Testing Page Fault Exception.............");
	volatile uint64_t *_test = reinterpret_cast<uint64_t*>(0x123);
	uint64_t _test2 = *_test;
	CConsole::Print("...OK!\n"); */
	
	// Testing PIC with PIT
/* 	CInterrupt::EnableInterrupts();
	CPIC::Unmask(0x00); */

	
	
	
	// Test GetPhysicalAddress
	uint64_t _TestVirtualAddress = (uint64_t)CLimine::GetFramebufferResponse()->framebuffers[0]->address;
	_TestVirtualAddress += 0x1234;
	uint64_t _TestPhysicalAddress = 0;
	CConsole::Print("Virtual 0x");
	CConsole::Print(htoa(_TestVirtualAddress, _TempText));
	_RetVal = CPaging::GetPhysicalAddress((void*)_TestVirtualAddress, (void**)&_TestPhysicalAddress);
	if (IS_ERROR(_RetVal)) {
		CConsole::Print("ERROR!\n");
	} else {
		CConsole::Print(" == Physical 0x");
		CConsole::Print(htoa(_TestPhysicalAddress, _TempText));
		CConsole::Print("\n");
	}
	
	
	CConsole::Print("Done!\n");
	while (true) {
		asm volatile ("hlt;\n");
	}
	
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
