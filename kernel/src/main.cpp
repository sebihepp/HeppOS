

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <liminestub.h>
#include <cstub.h>

#include <retvals.h>
#include <video/console.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <memory/paging.h>



char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);
char *htoa(uint64_t num, char* str);

extern "C" uint32_t kmain(void) {

	char _TempText[24];
	retval_t _RetVal = RETVAL_OK;
	
	_RetVal = Limine::Init();
	if (_RetVal != RETVAL_OK) {
		return _RetVal;
	}
		
	_RetVal = Console::Init(Limine::GetFramebufferResponse());
	if (_RetVal != RETVAL_OK) {
		return _RetVal;
	}
	
	Console::SetFGColor(0x00AAAAAA);
	Console::SetBGColor(0x00000000);
	Console::SetTitleFGColor(0x0000FFFF);
	Console::SetTitleBGColor(0x000000AA);
	Console::SetTitleText("HeppOS");
	Console::Clear();
	
	// Print Video Mode
	Console::Print("Video Format: ");
	Console::Print(itoa(Console::GetWidth(), _TempText, 10));
	Console::Print("x");
	Console::Print(itoa(Console::GetHeight(), _TempText, 10));
	Console::Print("x");
	Console::Print(itoa(Console::GetBPP(), _TempText, 10));
	Console::Print(" (Pitch=");
	Console::Print(itoa(Console::GetPitch(), _TempText, 10));
	Console::Print(")\n");
	
	//Debug Output
	
	/*
	// Print HHDM offset
	Console::Print("HHDM offset=0x");
	Console::Print(htoa(Limine::GetHHDMResponse()->offset, _TempText));
	Console::Print("\n");
	
	
	// Print Framebuffer Address
	Console::Print("Framebuffer Address=0x");
	Console::Print(htoa((uint64_t)Limine::GetFramebufferResponse()->framebuffers[0]->address, _TempText));
	Console::Print("\n");
	
	// Print CR3 Address
	Console::Print("CR3=0x");
	Console::Print(htoa((uint64_t)Paging::GetPhysicalAddress(NULL), _TempText));
	Console::Print("\n");
	
	// Print TSS Address
	Console::Print("TSS=0x");
	Console::Print(htoa((uint64_t)GDT::GetTSS(), _TempText));
	Console::Print("\n");
	*/
	
	
	Console::Print("Initializing GDT.........................");
	_RetVal = GDT::Init();
	if (_RetVal != RETVAL_OK) {
		Console::Print("ERROR!\n");
		return _RetVal;
	}
	Console::Print("...OK!\n");
	
	Console::Print("Loading GDT..............................");
	GDT::LoadGDT();
	Console::Print("...OK!\n");

	Console::Print("Loading TSS..............................");
	GDT::LoadTSS();
	Console::Print("...OK!\n");
	
	Console::Print("Initializing >IDT.........................");
	_RetVal = IDT::Init();
	if (_RetVal != RETVAL_OK) {
		Console::Print("ERROR!\n");
		return _RetVal;
	}
	Console::Print("...OK!\n");

	Console::Print("Loading IDT..............................");
	IDT::LoadIDT();
	Console::Print("...OK!\n");


	// Testing Cariage Return
	Console::Print("Testing Carriage Return..................ERROR!");
	Console::Print("\rTesting Carriage Return.....................OK!\n");
	
	// Testing tabulator
	Console::Print("Testing Tabulator...\tTEST\tTEST2\tTTT\tOK!\n");
	
	
	// Testing Exception
	Console::Print("Testing Exception 0......................");
	asm volatile (
		"int $0x0;\n"
	);
	Console::Print("...OK!\n");
	
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
