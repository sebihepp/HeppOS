

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <limine.h>
#include <cstub.h>

#include <retvals.h>
#include <console.h>


__attribute__((used, section(".requests"))) static volatile LIMINE_BASE_REVISION(2);
__attribute__((used, section(".requests"))) static volatile limine_framebuffer_request FramebufferRequest = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0,
	.response = NULL
};
__attribute__((used, section(".requests"))) static volatile limine_memmap_request MemoryMapRequest = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0,
	.response = NULL
};
__attribute__((used, section(".requests_start_marker"))) static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".requests_end_marker"))) static volatile LIMINE_REQUESTS_END_MARKER;

char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);

extern "C" uint32_t kmain(void) {

	char _TempText[16];
	retval_t _RetVal = RETVAL_OK;
	
	if (LIMINE_BASE_REVISION_SUPPORTED == false) {
		return RETVAL_ERROR_LIMINE_REV;
	}
	
	_RetVal = Console::Init(FramebufferRequest.response);
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
