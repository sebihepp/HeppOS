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

#pragma once

#include <stddef.h>
#include <stdint.h>

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif 

size_t kstrlen(const char *string) __attribute__((nothrow, const));
size_t kstrnlen(const char *string, size_t length) 
    __attribute__((nothrow, const));

char *kstrcpy(char *dest, const char *source) __attribute__((nothrow));
char *kstrncpy(char *dest, const char *source, size_t length) 
    __attribute__((nothrow));

int32_t kstrcmp(const char *string1, const char *string2) 
    __attribute__((nothrow, const));
int32_t kstrncmp(const char *string1, const char *string2, size_t length) 
    __attribute__((nothrow, const));

int32_t kstricmp(const char *string1, const char *string2) 
    __attribute__((nothrow, const));
int32_t kstrnicmp(const char *string1, const char *string2,
    size_t length) __attribute__((nothrow, const));

char *kstrcat(char *dest, const char *source) __attribute__((nothrow));
char *kstrncat(char *dest, const char *source,
               size_t length) __attribute__((nothrow));

char *kstrchr(const char *source, char character) 
    __attribute__((nothrow, const));
char *kstrrchr(const char *source, char character) 
    __attribute__((nothrow, const));

char *kstrpbrk(const char *source,
               const char *delimiter) __attribute__((nothrow, const));
size_t kstrspn(const char *source,
               const char *delimiter) __attribute__((nothrow, const));
size_t kstrcspn(const char *source,
                const char *delimiter) __attribute__((nothrow, const));
char *kstrstr(const char *string,
              const char *substring) __attribute__((nothrow, const));

char *kstrrev(char *string) __attribute__((nothrow));
char *kstrnrev(char *string, size_t length) __attribute__((nothrow));

char *kstrupr(char *string) __attribute__((nothrow));
char *kstrlwr(char *string) __attribute__((nothrow));

char* kitoa(int64_t number, char *string,
            uint32_t base) __attribute__((nothrow));
char* kutoa(uint64_t number, char *string,
            uint32_t base) __attribute__((nothrow));

char *ksprintf(char *dest, const char *format, ...) __attribute__((nothrow));
char *kvsprintf(char *dest, const char *format,
                va_list args) __attribute__((nothrow));
size_t ksnprintf(char *dest, size_t size, const char *format, ...)
    __attribute__((nothrow));
size_t kvsnprintf(char *dest, size_t size, const char *format,
                  va_list args) __attribute__((nothrow));


void *memchr(const void *source, int value, size_t size) 
    __attribute__(( nothrow ));
int32_t memcmp(const void* data1, const void* data2, size_t length) 
    __attribute__(( nothrow ));
void* memcpy(void *dest, const void *source, size_t length) 
    __attribute__(( nothrow ));
void* memmove(void *dest, const void *source, size_t length) 
    __attribute__(( nothrow ));
void* memset(void *dest, int value, size_t length) 
    __attribute__(( nothrow ));

#ifdef __cplusplus
}
#endif
