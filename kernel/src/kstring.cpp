
#include <kstring.h>


size_t kstrlen(const char *pString) {
	size_t i = 0;
	while (pString[i] != 0) {
		++i;
	}
	return i;
}

size_t kstrnlen(const char *pString, size_t pLength) {
	size_t _Count = 0;
	while (_Count < pLength) {
		if (pString[_Count] == 0)
			break;
		++_Count;
	}
	return _Count;
}

char *kstrcpy(char *pDest, const char *pSource) {
	size_t i = 0;
	while (pSource[i] != 0) {
		pDest[i] = pSource[i];
		++i;
	}
	pDest[i] = 0;
	return pDest;
}

char *kstrncpy(char *pDest, const char *pSource, size_t pLength) {
	size_t i = 0;
	while (i < pLength) {
		pDest[i] = pSource[i];
		if (pSource[i] == 0)
			break;
		++i;
	}
	while (i < pLength) {
		pDest[i] = 0;
		++i;
	}
	return pDest;
}

int32_t kstrcmp(const char *pString1, const char *pString2) {
	size_t i = 0;
	while ((pString1[i] == pString2[i]) && (pString1[i] != 0)) {
		++i;
	}
	return pString1[i] - pString2[i];
}


int32_t kstrncmp(const char *pString1, const char *pString2, size_t pLength) {
	size_t i = 0;
	while ((pString1[i] == pString2[i]) && (pString1[i] != 0)) {
		if (i >= pLength)
			break;
		++i;
	}
	return pString1[i] - pString2[i];
}


int32_t kstricmp(const char *pString1, const char *pString2) {
	size_t i = 0;
	while ((kto_lower(pString1[i]) == kto_lower(pString2[i])) && (pString1[i] != 0)) {
		++i;
	}
	return pString1[i] - pString2[i];
}

int32_t kstrnicmp(const char *pString1, const char *pString2, size_t pLength) {
	size_t i = 0;
	while ((kto_lower(pString1[i]) == kto_lower(pString2[i])) && (pString1[i] != 0)) {
		if (i >= pLength)
			break;
		++i;
	}
	return pString1[i] - pString2[i];
}


char *kstrcat(char *pDest, const char *pSource) {
	char *_target = pDest + kstrlen(pDest);
	size_t i = 0;
	while (pSource[i] != 0) {
		_target[i] = pSource[i];
		++i;
	}
	return pDest;
}


char *kstrncat(char *pDest, const char *pSource, size_t pLength) {
	char *_target = pDest + kstrlen(pDest);
	size_t i = 0;
	while ((pSource[i] != 0) && (i < pLength)) {
		_target[i] = pSource[i];
		++i;
	}
	while (i < pLength) {
		pDest[i] = 0;
		++i;
	}
	return pDest;
}

char *kstrchr(const char *pSource, char pChar) {
	size_t i = 0;
	while (pSource[i] != 0) {
		if (pSource[i] == pChar)
			return (char*)&pSource[i];
		++i;
	}
	return NULL;
}

char *kstrrchr(const char *pSource, char pChar) {
	size_t i = kstrlen(pSource) - 1;
	while (i > 0) {
		if (pSource[i] == pChar)
			return (char*)&pSource[i];
		--i;
	}
	return NULL;	
}



char *kstrpbrk(char *pSource, const char *pDelimiter) {
	size_t i = 0;
	size_t _DelimiterLength = kstrlen(pDelimiter);
	while (pSource[i] != 0) {
		for (size_t k = 0; k < _DelimiterLength; ++k) {
			if (pSource[i] == pDelimiter[k]) {
				return &pSource[i];
			}
		}
		++i;
	}
	return NULL;
}

const char *kstrpbrk(const char *pSource, const char *pDelimiter) {
	size_t i = 0;
	size_t _DelimiterLength = kstrlen(pDelimiter);
	while (pSource[i] != 0) {
		for (size_t k = 0; k < _DelimiterLength; ++k) {
			if (pSource[i] == pDelimiter[k]) {
				return &pSource[i];
			}
		}
		++i;
	}
	return NULL;
}

char *kstrnpbrk(char *pSource, const char *pDelimiter, size_t pLength) {
	size_t i = 0;
	size_t _DelimiterLength = kstrlen(pDelimiter);
	while ((pSource[i] != 0) && (i < pLength)) {
		for (size_t k = 0; k < _DelimiterLength; ++k) {
			if (pSource[i] == pDelimiter[k]) {
				return &pSource[i];
			}
		}
		++i;
	}
	return NULL;
}

const char *kstrnpbrk(const char *pSource, const char *pDelimiter, size_t pLength) {
	size_t i = 0;
	size_t _DelimiterLength = kstrlen(pDelimiter);
	while ((pSource[i] != 0) && (i < pLength)) {
		for (size_t k = 0; k < _DelimiterLength; ++k) {
			if (pSource[i] == pDelimiter[k]) {
				return &pSource[i];
			}
		}
		++i;
	}
	return NULL;
}



size_t kstrspn(const char *pSource, const char *pDelimiter) {
	size_t i = 0;
	size_t k = 0;
	while (pSource[i] != 0) {
		for (k = 0; k < kstrlen(pDelimiter); ++k) {
			if (pSource[i] == pDelimiter[k])
				break;
		}
		if (k >= kstrlen(pDelimiter))
			break;
		++i;
	}
	return i;
}

size_t kstrnspn(const char *pSource, const char *pDelimiter, size_t pLength) {
	size_t i = 0;
	size_t k = 0;
	while ((pSource[i] != 0) && (i < pLength)) {
		for (k = 0; k < kstrlen(pDelimiter); ++k) {
			if (pSource[i] == pDelimiter[k])
				break;
		}
		if (k >= kstrlen(pDelimiter))
			break;
		++i;
	}
	return i;
}


size_t kstrcspn(const char *pSource, const char *pDelimiter) {
	size_t i = 0;
	size_t k = 0;
	while (pSource[i] != 0) {
		for (k = 0; k < kstrlen(pDelimiter); ++k) {
			if (pSource[i] == pDelimiter[k])
				break;
		}
		if (k < kstrlen(pDelimiter))
			break;
		++i;
	}
	return i;
}

size_t kstrncspn(const char *pSource, const char *pDelimiter, size_t pLength) {
	size_t i = 0;
	size_t k = 0;
	while ((pSource[i] != 0) && (i < pLength)) {
		for (k = 0; k < kstrlen(pDelimiter); ++k) {
			if (pSource[i] == pDelimiter[k])
				break;
		}
		if (k < kstrlen(pDelimiter))
			break;
		++i;
	}
	return i;
}



char kto_lower(char pChar) {
	if ((pChar > 0x40) && (pChar < 0x5B)) {
		return pChar + 0x20;
	}
	return pChar;
}

char kto_upper(char pChar) {
	if ((pChar > 0x60) && (pChar < 0x7B)) {
		return pChar - 0x20;
	}
	return pChar;
}

char* itoa(int64_t num, char* str, int base)
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

char *utoa (uint64_t value, char *str, int base) {
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



#ifdef __cplusplus
extern "C" {
#endif


void *memchr(const void *pSource, int8_t pValue, size_t pSize) {
	uint64_t i;
	if (pSize == 0)
		return NULL;
	while (i < pSize) {
		if (((int8_t*)pSource)[i] == pValue) {
			return &((int8_t*)pSource)[i];
		}
		++i;
	}
	return NULL;
}

int32_t memcmp(const void *a, const void *b, size_t length)
{
	const uint8_t *_a = (const uint8_t*)a;
	const uint8_t *_b = (const uint8_t*)b;
	
	for (size_t i = 0; i < length; i++)
	{
		if (_a[i] < _b[i]) {
			return -1;
		} else if (_a[i] > _b[i]) {
			return 1; 
		}
	}
	return 0;
}

void *memcpy(void *dest, const void *src, size_t length)
{
	uint8_t *_target = (uint8_t*)dest;
	for (size_t i = 0; i < length; i++)
	{
		_target[i] = ((uint8_t*)src)[i];
	}
	return dest;
}

void *memmove(void *dest, const void *source, size_t length)
{
	uint8_t *_target = (uint8_t*)dest;
	if (dest < source) 
	{
		for (size_t i = 0; i < length; i++)
		{
			_target[i] = ((uint8_t*)source)[i];
		}
	} else {
		for (size_t i = length; i != 0; i--)
		{
			_target[i-1] = ((uint8_t*)source)[i-1];
		}		
	}
	return dest;
}

void *memset(void *s, int8_t value, size_t length)
{
	uint8_t *_target = (uint8_t*)s;
	for (size_t i = 0; i < length; i++)
	{
		_target[i] = (uint8_t) value;
	}
	return s;
}


#ifdef __cplusplus
}
#endif
