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

#include <stdarg.h>


#include <kstring.h>
#include <ktype.h>


size_t kstrlen(const char *pString) {
	if (pString == NULL)
		return 0;
	
	size_t i = 0;
	while (pString[i] != 0) {
		++i;
	}
	return i;
}

size_t kstrnlen(const char *pString, size_t pLength) {
	if (pString == NULL)
		return 0;
	
	size_t _Count = 0;
	while (_Count < pLength) {
		if (pString[_Count] == 0)
			break;
		++_Count;
	}
	return _Count;
}

char *kstrcpy(char *pDest, const char *pSource) {
	if (pDest == NULL)
		return NULL;
	if (pSource == NULL)
		return pDest;
	
	size_t i = 0;
	while (pSource[i] != 0) {
		pDest[i] = pSource[i];
		++i;
	}
	pDest[i] = 0;
	return pDest;
}

char *kstrncpy(char *pDest, const char *pSource, size_t pLength) {
	if (pDest == NULL)
		return NULL;
	if (pSource == NULL)
		return pDest;

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
	while ((ktolower(pString1[i]) == ktolower(pString2[i])) && (pString1[i] != 0)) {
		++i;
	}
	return pString1[i] - pString2[i];
}

int32_t kstrnicmp(const char *pString1, const char *pString2, size_t pLength) {
	size_t i = 0;
	while ((ktolower(pString1[i]) == ktolower(pString2[i])) && (pString1[i] != 0)) {
		if (i >= pLength)
			break;
		++i;
	}
	return pString1[i] - pString2[i];
}


char *kstrcat(char *pDest, const char *pSource) {
	if (pDest == NULL)
		return NULL;
	if (pSource == NULL)
		return pDest;
	
	char *_target = pDest + kstrlen(pDest);
	size_t i = 0;
	while (pSource[i] != 0) {
		_target[i] = pSource[i];
		++i;
	}
	_target[i] = 0;
	return pDest;
}


char *kstrncat(char *pDest, const char *pSource, size_t pLength) {
	if (pDest == NULL)
		return NULL;
	if (pSource == NULL)
		return pDest;

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
	if (pSource == NULL)
		return NULL;
	
	size_t i = 0;
	while (pSource[i] != 0) {
		if (pSource[i] == pChar)
			return (char*)&pSource[i];
		++i;
	}
	return NULL;
}

char *kstrrchr(const char *pSource, char pChar) {
	if (pSource == NULL)
		return NULL;
	
	size_t i = kstrlen(pSource) - 1;
	while (i > 0) {
		if (pSource[i] == pChar)
			return (char*)&pSource[i];
		--i;
	}
	return NULL;	
}



char *kstrpbrk(const char *pSource, const char *pDelimiter) {
	if (pSource == NULL)
		return NULL;
	if (pDelimiter == NULL)
		return (char*)pSource;
	
	size_t i = 0;
	size_t _DelimiterLength = kstrlen(pDelimiter);
	while (pSource[i] != 0) {
		for (size_t k = 0; k < _DelimiterLength; ++k) {
			if (pSource[i] == pDelimiter[k]) {
				return (char*)(&pSource[i]);
			}
		}
		++i;
	}
	return NULL;
}


size_t kstrspn(const char *pSource, const char *pDelimiter) {
	if (pSource == NULL)
		return 0;
	if (pDelimiter == NULL)
		return kstrlen(pSource);

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


size_t kstrcspn(const char *pSource, const char *pDelimiter) {
	if (pSource == NULL)
		return 0;
	if (pDelimiter == NULL)
		return 0;

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

char *kstrstr(const char *pString, const char *pSubString) {
	if (pString == NULL)
		return NULL;
	if (pSubString == NULL)
		return NULL;
	
	size_t i = 0;
	
	while (pString[i] != 0) {
		
		if (kstrncmp(&pString[i], pSubString, kstrlen(pSubString) - 1) == 0)
			return ((char*)pString+i);
			
		++i;
	}
	
	return NULL;
}


char *kitoa(int64_t pValue, char *pString, uint32_t pBase) {
	
	const char *_Digit = "0123456789abcdefghijklmnopqrstuvwxyz";
	bool _Negative = false;
	size_t i = 0;
	
	if ((pBase < 2) || (pBase > 36))
		return NULL;
 
	if (pValue < 0) {
		_Negative = true;
		pValue = -pValue;
	}
	
	if (pValue == 0) {
		pString[i++] = '0';
		pString[i] = 0;
		return pString;
	}
	
	while (pValue != 0) {
		uint64_t _Remainder = pValue % pBase;
		pString[i++] = _Digit[_Remainder];
		pValue /= pBase;
	}
	
	if (_Negative) {
		pString[i++] = '-';
	}
	pString[i] = 0;
	
	return kstrrev(pString);
}

char* kutoa(uint64_t pNumber, char *pString, uint32_t pBase) {
	
	const char *_Digit = "0123456789abcdefghijklmnopqrstuvwxyz";
	size_t i = 0;
	
	if ((pBase < 2) || (pBase > 36))
		return NULL;
 
	
	if (pNumber == 0) {
		pString[i++] = '0';
		pString[i] = 0;
		return pString;
	}
	
	while (pNumber != 0) {
		uint64_t _Remainder = pNumber % pBase;
		pString[i++] = _Digit[_Remainder];
		pNumber /= pBase;
	}
	
	pString[i] = 0;
	
	return kstrrev(pString);
}


char *ksprintf(char *pDest, const char *pFormat, ...) {
	if (pDest == NULL)
		return NULL;
	if (pFormat == NULL)
		return pDest;
	
	va_list _ap;
	va_start(_ap, pFormat);
	kvsprintf(pDest, pFormat, _ap);
	va_end(_ap);
	
	return pDest;
}

char *kvsprintf(char *pDest, const char *pFormat, va_list pArgs) {
	if (pDest == NULL)
		return NULL;
	if (pFormat == NULL)
		return pDest;
		
	static char _Buffer[128];
	size_t i = 0;
	size_t k = 0;
	bool _Special = false;
	bool _Long = false;
	int64_t _Width = 0;
	bool _LeadingZero = false;
	int64_t _LeadingWidth = 0;
	int64_t _NumberLength = 0;
	
	while (pFormat[i] != 0) {
		
	// ToDo: Implement
		if (_Special) {
			
			switch (pFormat[i]) {
				case '0':
					if (_Width == 0) {
						_LeadingZero = true;
					} else {
						_Width *= 10;
					}
					break;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					_Width *= 10;
					_Width += pFormat[i] - '0';
					break;
				case 'l':
					_Long = true;
					break;
				case '%':
					pDest[k++] = '%';
					
					_Special = false;
					_Width = 0;
					_Long = false;
					_LeadingZero = false;
					_NumberLength = 0;
					break;
				case 'c':
					pDest[k++] = va_arg(pArgs, int);
					
					_Special = false;
					_Width = 0;
					_Long = false;
					_LeadingZero = false;
					_NumberLength = 0;
					break;
				case 's':
					pDest[k] = 0;
					kstrcat(pDest, va_arg(pArgs, const char*));
					k = kstrlen(pDest);
					
					_Special = false;
					_Width = 0;
					_Long = false;
					_LeadingZero = false;
					_NumberLength = 0;
					break;
				case 'i':
				case 'd':
					if (_Long) {
						kitoa(va_arg(pArgs, int64_t), _Buffer, 10);
					} else {
						kitoa(va_arg(pArgs, int), _Buffer, 10);
					}
					_LeadingWidth = _Width - kstrlen(_Buffer);
					if (_Buffer[0] == '-') {
						pDest[k++] = '-';
						kstrcpy(_Buffer, &_Buffer[1]);
					}
					if (_LeadingZero) {
						for (int64_t l = 0; l < _LeadingWidth; ++l) {
							pDest[k++] = '0';
						}
					}
					pDest[k] = 0;
					kstrcat(pDest, _Buffer);
					k = kstrlen(pDest);
					if (_LeadingZero == false) {
						for (int64_t l = 0; l < _LeadingWidth; ++l) {
							pDest[k++] = ' ';
						}						
					}
					
					_Special = false;
					_Width = 0;
					_Long = false;
					_LeadingZero = false;
					_NumberLength = 0;
					break;
				case 'p':
					pDest[k++] = '0';
					pDest[k++] = 'x';
					kutoa((uintptr_t)va_arg(pArgs, uintptr_t), _Buffer, 16);
					pDest[k] = 0;
					kstrcat(pDest, kstrlwr(_Buffer));
					k = kstrlen(pDest);
					
					_Special = false;
					_Width = 0;
					_Long = false;
					_LeadingZero = false;
					_NumberLength = 0;
					break;				
				case 'x':
					if (_Long) {
						kutoa(va_arg(pArgs, uint64_t), _Buffer, 16);
					} else {
						kutoa(va_arg(pArgs, unsigned int), _Buffer, 16);
					}
					_LeadingWidth = _Width - kstrlen(_Buffer);
					if (_LeadingZero) {
						for (int64_t l = 0; l < _LeadingWidth; ++l) {
							pDest[k++] = '0';
						}
					}
					pDest[k] = 0;
					kstrcat(pDest, kstrlwr(_Buffer));
					k = kstrlen(pDest);
					if (_LeadingZero == false) {
						for (int64_t l = 0; l < _LeadingWidth; ++l) {
							pDest[k++] = ' ';
						}						
					}
					
					_Special = false;
					_Width = 0;
					_Long = false;
					_LeadingZero = false;
					_NumberLength = 0;
					break;
				case 'X':
					if (_Long) {
						kutoa(va_arg(pArgs, uint64_t), _Buffer, 16);
					} else {
						kutoa(va_arg(pArgs, unsigned int), _Buffer, 16);
					}
					_LeadingWidth = _Width - kstrlen(_Buffer);
					if (_LeadingZero) {
						for (int64_t l = 0; l < _LeadingWidth; ++l) {
							pDest[k++] = '0';
						}
					}
					pDest[k] = 0;
					kstrcat(pDest, kstrupr(_Buffer));
					k = kstrlen(pDest);
					if (_LeadingZero == false) {
						for (int64_t l = 0; l < _LeadingWidth; ++l) {
							pDest[k++] = ' ';
						}						
					}
					
					_Special = false;
					_Width = 0;
					_Long = false;
					_LeadingZero = false;
					_NumberLength = 0;
					break;
				case 'o':
					if (_Long) {
						kitoa(va_arg(pArgs, int64_t), _Buffer, 8);
					} else {
						kitoa(va_arg(pArgs, int), _Buffer, 8);
					}
					_LeadingWidth = _Width - kstrlen(_Buffer);
					if (_Buffer[0] == '-') {
						pDest[k++] = '-';
						kstrcpy(_Buffer, &_Buffer[1]);
					}
					if (_LeadingZero) {
						for (int64_t l = 0; l < _LeadingWidth; ++l) {
							pDest[k++] = '0';
						}
					}
					pDest[k] = 0;
					kstrcat(pDest, _Buffer);
					k = kstrlen(pDest);
					if (_LeadingZero == false) {
						for (int64_t l = 0; l < _LeadingWidth; ++l) {
							pDest[k++] = ' ';
						}						
					}
					
					_Special = false;
					_Width = 0;
					_Long = false;
					_LeadingZero = false;
					_NumberLength = 0;
					break;
				default:
					_Special = false;
					_Width = 0;
					_Long = false;
					_LeadingZero = false;
					_NumberLength = 0;
					break;
			}
			
		} else {
			
			if (pFormat[i] == '%') {
				_Special = true;
				_Width = 0;
				_Long = false;
				_LeadingZero = false;
				_NumberLength = 0;
			} else {
				pDest[k++] = pFormat[i];
			}
			
		}
	
		++i;
	}
	
	pDest[k] = 0;
	
	return pDest;
	
}


char *kstrupr(char *pString) {
	if (pString == NULL)
		return NULL;
	
	size_t i = 0;
	while (pString[i] != 0) {
		pString[i] = ktoupper(pString[i]);
		++i;
	}
	
	return pString;
}

char *kstrlwr(char *pString) {
	if (pString == NULL)
		return NULL;
	
	size_t i = 0;
	while (pString[i] != 0) {
		pString[i] = ktolower(pString[i]);
		++i;
	}
	
	return pString;	
}



char *kstrrev(char *pString) {
	if (pString == NULL)
		return NULL;
	
	size_t i = 0;
	size_t k = kstrlen(pString) - 1;
	char c;
	
	while (i < k) {
		c = pString[i];
		pString[i] = pString[k];
		pString[k] = c;
		
		++i;
		--k;
	}
	
	return pString;
}

char *kstrnrev(char *pString, size_t pLength) {
	if (pString == NULL)
		return NULL;
	
	size_t i = 0;
	size_t k = kstrnlen(pString, pLength) - 1;
	char c;
	
	while (i < k) {
		c = pString[i];
		pString[i] = pString[k];
		pString[k] = c;
		
		++i;
		--k;
	}
	
	return pString;	
}




#ifdef __cplusplus
extern "C" {
#endif


void *memchr(const void *pSource, int pValue, size_t pSize) {
	uint64_t i = 0;
	if (pSize == 0)
		return NULL;
	while (i < pSize) {
		if (((int8_t*)pSource)[i] == (int8_t)pValue) {
			return &((int8_t*)pSource)[i];
		}
		++i;
	}
	return NULL;
}

int32_t memcmp(const void *pData1, const void *pData2, size_t pLength)
{
	const uint8_t *_a = (const uint8_t*)pData1;
	const uint8_t *_b = (const uint8_t*)pData2;
	
	for (size_t i = 0; i < pLength; i++)
	{
		if (_a[i] < _b[i]) {
			return -1;
		} else if (_a[i] > _b[i]) {
			return 1; 
		}
	}
	return 0;
}

void *memcpy(void *pDest, const void *pSource, size_t pLength)
{
	uint8_t *_target = (uint8_t*)pDest;
	for (size_t i = 0; i < pLength; i++)
	{
		_target[i] = ((uint8_t*)pSource)[i];
	}
	return pDest;
}

void *memmove(void *pDest, const void *pSource, size_t pLength)
{
	uint8_t *_target = (uint8_t*)pDest;
	if (pDest < pSource) 
	{
		for (size_t i = 0; i < pLength; i++)
		{
			_target[i] = ((uint8_t*)pSource)[i];
		}
	} else {
		for (size_t i = pLength; i != 0; i--)
		{
			_target[i-1] = ((uint8_t*)pSource)[i-1];
		}		
	}
	return pDest;
}

void *memset(void *pDest, int pValue, size_t pLength)
{
	uint8_t *_target = (uint8_t*)pDest;
	for (size_t i = 0; i < pLength; i++)
	{
		_target[i] = (uint8_t)pValue;
	}
	return pDest;
}


#ifdef __cplusplus
}
#endif
