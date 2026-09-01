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


size_t kstrlen(const char *string) {
	if (string == NULL)
		return 0;
	
	size_t i = 0;
	while (string[i] != 0) {
		++i;
	}
	return i;
}

size_t kstrnlen(const char *string, size_t length) {
	if (string == NULL)
		return 0;
	
	size_t count = 0;
	while (count < length) {
		if (string[count] == 0)
			break;
		++count;
	}
	return count;
}

char *kstrcpy(char *dest, const char *source) {
	if (dest == NULL)
		return NULL;
	if (source == NULL)
		return dest;
	
	size_t i = 0;
	while (source[i] != 0) {
		dest[i] = source[i];
		++i;
	}
	dest[i] = 0;
	return dest;
}

char *kstrncpy(char *dest, const char *source, size_t length) {
	if (dest == NULL)
		return NULL;
	if (source == NULL)
		return dest;

	size_t i = 0;
	while (i < length) {
		dest[i] = source[i];
		if (source[i] == 0)
			break;
		++i;
	}
	while (i < length) {
		dest[i] = 0;
		++i;
	}
	return dest;
}

int32_t kstrcmp(const char *string1, const char *string2) {
	size_t i = 0;
	while ((string1[i] == string2[i]) && (string1[i] != 0)) {
		++i;
	}
	return string1[i] - string2[i];
}


int32_t kstrncmp(const char *string1, const char *string2, size_t length) {
	size_t i = 0;
	while ((string1[i] == string2[i]) && (string1[i] != 0)) {
		if (i >= length)
			break;
		++i;
	}
	return string1[i] - string2[i];
}


int32_t kstricmp(const char *string1, const char *string2) {
	size_t i = 0;
	while ((ktolower(string1[i]) == ktolower(string2[i])) && (string1[i] != 0)) {
		++i;
	}
	return string1[i] - string2[i];
}

int32_t kstrnicmp(const char *string1, const char *string2, size_t length) {
	size_t i = 0;
	while ((ktolower(string1[i]) == ktolower(string2[i])) && (string1[i] != 0)) {
		if (i >= length)
			break;
		++i;
	}
	return string1[i] - string2[i];
}


char *kstrcat(char *dest, const char *source) {
	if (dest == NULL)
		return NULL;
	if (source == NULL)
		return dest;
	
	char *target = dest + kstrlen(dest);
	size_t i = 0;
	while (source[i] != 0) {
		target[i] = source[i];
		++i;
	}
	target[i] = 0;
	return dest;
}


char *kstrncat(char *dest, const char *source, size_t length) {
	if (dest == NULL)
		return NULL;
	if (source == NULL)
		return dest;

	char *target = dest + kstrlen(dest);
	size_t i = 0;
	while ((source[i] != 0) && (i < length)) {
		target[i] = source[i];
		++i;
	}
	target[i] = 0;
	return dest;
}

char *kstrchr(const char *source, char character) {
	if (source == NULL)
		return NULL;
	
	size_t i = 0;
	while (source[i] != 0) {
		if (source[i] == character)
			return (char*)&source[i];
		++i;
	}
	return NULL;
}

char *kstrrchr(const char *source, char character) {
	if (source == NULL)
		return NULL;
	
	size_t i = kstrlen(source) - 1;
	while (i > 0) {
		if (source[i] == character)
			return (char*)&source[i];
		--i;
	}
	return NULL;	
}



char *kstrpbrk(const char *source, const char *delimiter) {
	if (source == NULL)
		return NULL;
	if (delimiter == NULL)
		return (char*)source;
	
	size_t i = 0;
	size_t delimiter_length = kstrlen(delimiter);
	while (source[i] != 0) {
		for (size_t k = 0; k < delimiter_length; ++k) {
			if (source[i] == delimiter[k]) {
				return (char*)(&source[i]);
			}
		}
		++i;
	}
	return NULL;
}


size_t kstrspn(const char *source, const char *delimiter) {
	if (source == NULL)
		return 0;
	if (delimiter == NULL)
		return kstrlen(source);

	size_t i = 0;
	size_t k = 0;
	while (source[i] != 0) {
		for (k = 0; k < kstrlen(delimiter); ++k) {
			if (source[i] == delimiter[k])
				break;
		}
		if (k >= kstrlen(delimiter))
			break;
		++i;
	}
	return i;
}


size_t kstrcspn(const char *source, const char *delimiter) {
	if (source == NULL)
		return 0;
	if (delimiter == NULL)
		return 0;

	size_t i = 0;
	size_t k = 0;
	while (source[i] != 0) {
		for (k = 0; k < kstrlen(delimiter); ++k) {
			if (source[i] == delimiter[k])
				break;
		}
		if (k < kstrlen(delimiter))
			break;
		++i;
	}
	return i;
}

char *kstrstr(const char *string, const char *sub_string) {
	if (string == NULL)
		return NULL;
	if (sub_string == NULL)
		return NULL;
	
	size_t i = 0;
	
	while (string[i] != 0) {
		
		if (kstrncmp(&string[i], sub_string, kstrlen(sub_string) - 1) == 0)
			return ((char*)string+i);
			
		++i;
	}
	
	return NULL;
}


char *kitoa(int64_t value, char *string, uint32_t base) {
	
	const char *digit = "0123456789abcdefghijklmnopqrstuvwxyz";
	bool negative = false;
	size_t i = 0;
	
	if ((base < 2) || (base > 36))
		return NULL;
 
	if (value < 0) {
		negative = true;
		value = -value;
	}
	
	if (value == 0) {
		string[i++] = '0';
		string[i] = 0;
		return string;
	}
	
	while (value != 0) {
		uint64_t remainder = value % base;
		string[i++] = digit[remainder];
		value /= base;
	}
	
	if (negative) {
		string[i++] = '-';
	}
	string[i] = 0;
	
	return kstrrev(string);
}

char* kutoa(uint64_t number, char *string, uint32_t base) {
	
	const char *digit = "0123456789abcdefghijklmnopqrstuvwxyz";
	size_t i = 0;
	
	if ((base < 2) || (base > 36))
		return NULL;
 
	
	if (number == 0) {
		string[i++] = '0';
		string[i] = 0;
		return string;
	}
	
	while (number != 0) {
		uint64_t remainder = number % base;
		string[i++] = digit[remainder];
		number /= base;
	}
	
	string[i] = 0;
	
	return kstrrev(string);
}


char *ksprintf(char *dest, const char *format, ...) {
	if (dest == NULL)
		return NULL;
	if (format == NULL)
		return dest;
	
	va_list ap;
	va_start(ap, format);
	kvsprintf(dest, format, ap);
	va_end(ap);
	
	return dest;
}

char *kvsprintf(char *dest, const char *format, va_list args) {

	if (dest == NULL)
		return NULL;
	if (format == NULL)
		return dest;
		
	static char buffer[128];
	size_t i = 0;
	size_t k = 0;
	bool is_special = false;
	bool is_long = false;
	int64_t width = 0;
	bool is_leading_zero = false;
	int64_t leading_width = 0;
	
	while (format[i] != 0) {
		
	// ToDo: Implement
		if (is_special) {
			
			switch (format[i]) {
				case '0':
					if (width == 0) {
						is_leading_zero = true;
					} else {
						width *= 10;
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
					width *= 10;
					width += format[i] - '0';
					break;
				case 'l':
					is_long = true;
					break;
				case '%':
					dest[k++] = '%';
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'c':
					dest[k++] = va_arg(args, int);
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 's':
					dest[k] = 0;
					kstrcat(dest, va_arg(args, const char*));
					k = kstrlen(dest);
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'i':
				case 'd':
					if (is_long) {
						kitoa(va_arg(args, int64_t), buffer, 10);
					} else {
						kitoa(va_arg(args, int32_t), buffer, 10);
					}
					leading_width = width - kstrlen(buffer);
					if (buffer[0] == '-') {
						dest[k++] = '-';
						kstrcpy(buffer, &buffer[1]);
					}
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
						}
					}
					dest[k] = 0;
					kstrcat(dest, buffer);
					k = kstrlen(dest);
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'u':
					if (is_long) {
						kutoa(va_arg(args, uint64_t), buffer, 10);
					} else {
						kutoa(va_arg(args, uint32_t), buffer, 10);
					}
					leading_width = width - kstrlen(buffer);
					if (buffer[0] == '-') {
						dest[k++] = '-';
						kstrcpy(buffer, &buffer[1]);
					}
					dest[k] = 0;
					kstrcat(dest, buffer);
					k = kstrlen(dest);
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
						}
					}
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'p':
					dest[k++] = '0';
					dest[k++] = 'x';
					kutoa((uintptr_t)va_arg(args, uintptr_t), buffer, 16);
					leading_width = width - kstrlen(buffer);
					if (buffer[0] == '-') {
						dest[k++] = '-';
						kstrcpy(buffer, &buffer[1]);
					}
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
						}
					}
					dest[k] = 0;
					kstrcat(dest, buffer);
					k = kstrlen(dest);
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;				
				case 'x':
					if (is_long) {
						kutoa(va_arg(args, uint64_t), buffer, 16);
					} else {
						kutoa(va_arg(args, uint32_t), buffer, 16);
					}
					leading_width = width - kstrlen(buffer);
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
						}
					}
					dest[k] = 0;
					kstrcat(dest, kstrlwr(buffer));
					k = kstrlen(dest);
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'X':
					if (is_long) {
						kutoa(va_arg(args, uint64_t), buffer, 16);
					} else {
						kutoa(va_arg(args, uint32_t), buffer, 16);
					}
					leading_width = width - kstrlen(buffer);
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
						}
					}
					dest[k] = 0;
					kstrcat(dest, kstrupr(buffer));
					k = kstrlen(dest);
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'o':
					if (is_long) {
						kitoa(va_arg(args, int64_t), buffer, 8);
					} else {
						kitoa(va_arg(args, int32_t), buffer, 8);
					}
					leading_width = width - kstrlen(buffer);
					if (buffer[0] == '-') {
						dest[k++] = '-';
						kstrcpy(buffer, &buffer[1]);
					}
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
						}
					}
					dest[k] = 0;
					kstrcat(dest, buffer);
					k = kstrlen(dest);
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				default:
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
			}
						
		} else {
			
			if (format[i] == '%') {
				is_special = true;
				width = 0;
				is_long = false;
				is_leading_zero = false;
			} else {
				dest[k++] = format[i];
			}
			
		}
	
		++i;
	}
	
	dest[k] = 0;

	return dest;
}

size_t ksnprintf(char *dest, size_t size, const char *format, ...) {
	if (dest == NULL)
		return 0;
	if (format == NULL)
		return 0;
	
	va_list ap;
	va_start(ap, format);
	int64_t return_value = kvsnprintf(dest, size, format, ap);
	va_end(ap);
	
	return return_value;
}

#define CHECK_SNPRINTF_SIZE { \
	if (k >= size) { \
		dest[size-1] = 0; \
		return size; \
	} }

#define COPY_AND_CHECK_SNPRINTF_SIZE(SOURCE) { \
	dest[k] = 0; \
	kstrncat(dest, (SOURCE), size - k - 1); \
	dest[size-1] = 0; \
	k = kstrlen(dest); \
	if ((k+1) >= size) { \
		dest[size-1] = 0; \
		return size; \
	} }

size_t kvsnprintf(char *dest, size_t size, const char *format, va_list args) {

	if (dest == NULL)
		return 0;
	if (format == NULL)
		return 0;
		
	static char buffer[128];
	size_t i = 0;
	size_t k = 0;
	bool is_special = false;
	bool is_long = false;
	int64_t width = 0;
	bool is_leading_zero = false;
	int64_t leading_width = 0;
	
	while (format[i] != 0) {
		
	// ToDo: Implement
		if (is_special) {
			
			switch (format[i]) {
				case '0':
					if (width == 0) {
						is_leading_zero = true;
					} else {
						width *= 10;
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
					width *= 10;
					width += format[i] - '0';
					break;
				case 'l':
					is_long = true;
					break;
				case '%':
					dest[k++] = '%';
					CHECK_SNPRINTF_SIZE;

					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'c':
					dest[k++] = va_arg(args, int);
					CHECK_SNPRINTF_SIZE;

					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 's':
					COPY_AND_CHECK_SNPRINTF_SIZE(va_arg(args, const char*));

					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'i':
				case 'd':
					if (is_long) {
						kitoa(va_arg(args, int64_t), buffer, 10);
					} else {
						kitoa(va_arg(args, int32_t), buffer, 10);
					}
					leading_width = width - kstrlen(buffer);
					if (buffer[0] == '-') {
						dest[k++] = '-';
						CHECK_SNPRINTF_SIZE;
						kstrcpy(buffer, &buffer[1]);
					}
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
							CHECK_SNPRINTF_SIZE;
						}
					}
					COPY_AND_CHECK_SNPRINTF_SIZE(buffer);
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
							CHECK_SNPRINTF_SIZE;
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'u':
					if (is_long) {
						kutoa(va_arg(args, uint64_t), buffer, 10);
					} else {
						kutoa(va_arg(args, uint32_t), buffer, 10);
					}
					leading_width = width - kstrlen(buffer);
					if (buffer[0] == '-') {
						dest[k++] = '-';
						CHECK_SNPRINTF_SIZE;
						kstrcpy(buffer, &buffer[1]);
					}
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
							CHECK_SNPRINTF_SIZE;
						}
					}
					COPY_AND_CHECK_SNPRINTF_SIZE(buffer);
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
							CHECK_SNPRINTF_SIZE;
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'p':
					dest[k++] = '0';
					dest[k++] = 'x';
					kutoa((uintptr_t)va_arg(args, uintptr_t), buffer, 16);
					leading_width = width - kstrlen(buffer);
					if (buffer[0] == '-') {
						dest[k++] = '-';
						CHECK_SNPRINTF_SIZE;
						kstrcpy(buffer, &buffer[1]);
					}
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
							CHECK_SNPRINTF_SIZE;
						}
					}
					COPY_AND_CHECK_SNPRINTF_SIZE(buffer);
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
							CHECK_SNPRINTF_SIZE;
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;				
				case 'x':
					if (is_long) {
						kutoa(va_arg(args, uint64_t), buffer, 16);
					} else {
						kutoa(va_arg(args, uint32_t), buffer, 16);
					}
					leading_width = width - kstrlen(buffer);
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
							CHECK_SNPRINTF_SIZE;
						}
					}
					COPY_AND_CHECK_SNPRINTF_SIZE(buffer);
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
							CHECK_SNPRINTF_SIZE;
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'X':
					if (is_long) {
						kutoa(va_arg(args, uint64_t), buffer, 16);
					} else {
						kutoa(va_arg(args, uint32_t), buffer, 16);
					}
					leading_width = width - kstrlen(buffer);
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
							CHECK_SNPRINTF_SIZE;
						}
					}
					COPY_AND_CHECK_SNPRINTF_SIZE(buffer);
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
							CHECK_SNPRINTF_SIZE;
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				case 'o':
					if (is_long) {
						kitoa(va_arg(args, int64_t), buffer, 8);
					} else {
						kitoa(va_arg(args, int32_t), buffer, 8);
					}
					leading_width = width - kstrlen(buffer);
					if (buffer[0] == '-') {
						dest[k++] = '-';
						CHECK_SNPRINTF_SIZE;
						kstrcpy(buffer, &buffer[1]);
					}
					if (is_leading_zero) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = '0';
							CHECK_SNPRINTF_SIZE;
						}
					}
					COPY_AND_CHECK_SNPRINTF_SIZE(buffer);
					if (is_leading_zero == false) {
						for (int64_t l = 0; l < leading_width; ++l) {
							dest[k++] = ' ';
							CHECK_SNPRINTF_SIZE;
						}						
					}
					
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
				default:
					is_special = false;
					width = 0;
					is_long = false;
					is_leading_zero = false;
					break;
			}
			
		} else {
			
			if (format[i] == '%') {
				is_special = true;
				width = 0;
				is_long = false;
				is_leading_zero = false;
			} else {
				dest[k++] = format[i];
				CHECK_SNPRINTF_SIZE;
			}
			
		}
	
		++i;
	}
	
	dest[k] = 0;

	return k;
}


char *kstrupr(char *string) {
	if (string == NULL)
		return NULL;
	
	size_t i = 0;
	while (string[i] != 0) {
		string[i] = ktoupper(string[i]);
		++i;
	}
	
	return string;
}

char *kstrlwr(char *string) {
	if (string == NULL)
		return NULL;
	
	size_t i = 0;
	while (string[i] != 0) {
		string[i] = ktolower(string[i]);
		++i;
	}
	
	return string;	
}



char *kstrrev(char *string) {
	if (string == NULL)
		return NULL;
	
	size_t i = 0;
	size_t k = kstrlen(string) - 1;
	char c;
	
	while (i < k) {
		c = string[i];
		string[i] = string[k];
		string[k] = c;
		
		++i;
		--k;
	}
	
	return string;
}

char *kstrnrev(char *string, size_t length) {
	if (string == NULL)
		return NULL;
	
	size_t i = 0;
	size_t k = kstrnlen(string, length) - 1;
	char c;
	
	while (i < k) {
		c = string[i];
		string[i] = string[k];
		string[k] = c;
		
		++i;
		--k;
	}
	
	return string;	
}




#ifdef __cplusplus
extern "C" {
#endif


void *memchr(const void *source, int value, size_t size) {
	uint64_t i = 0;
	if (size == 0)
		return NULL;
	while (i < size) {
		if (((int8_t*)source)[i] == (int8_t)value) {
			return &((int8_t*)source)[i];
		}
		++i;
	}
	return NULL;
}

int32_t memcmp(const void *data1, const void *data2, size_t length)
{
	const uint8_t *a = (const uint8_t*)data1;
	const uint8_t *b = (const uint8_t*)data2;
	
	for (size_t i = 0; i < length; i++)
	{
		if (a[i] < b[i]) {
			return -1;
		} else if (a[i] > b[i]) {
			return 1; 
		}
	}
	return 0;
}

void *memcpy(void *dest, const void *source, size_t length)
{
	uint8_t *target = (uint8_t*)dest;
	for (size_t i = 0; i < length; i++)
	{
		target[i] = ((uint8_t*)source)[i];
	}
	return dest;
}

void *memmove(void *dest, const void *source, size_t length)
{
	uint8_t *target = (uint8_t*)dest;
	if (dest < source) 
	{
		for (size_t i = 0; i < length; i++)
		{
			target[i] = ((uint8_t*)source)[i];
		}
	} else {
		for (size_t i = length; i != 0; i--)
		{
			target[i-1] = ((uint8_t*)source)[i-1];
		}		
	}
	return dest;
}

void *memset(void *dest, int value, size_t length)
{
	uint8_t *target = (uint8_t*)dest;
	for (size_t i = 0; i < length; i++)
	{
		target[i] = (uint8_t)value;
	}
	return dest;
}


#ifdef __cplusplus
}
#endif
