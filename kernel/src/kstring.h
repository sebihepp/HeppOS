
#ifndef HEADER_KSTRING
#define HEADER_KSTRING

#include <stddef.h>
#include <stdint.h>


size_t kstrlen(const char *pString) __attribute__ (( nothrow ));

char *kstrcpy(char *pDest, const char *pSource) __attribute__ (( nothrow ));
char *kstrncpy(char *pDest, const char *pSource, size_t pLength) __attribute__ (( nothrow ));

int32_t kstrcmp(const char *pString1, const char *pString2) __attribute__ (( nothrow ));
int32_t kstrncmp(const char *pString1, const char *pString2, size_t pLength) __attribute__ (( nothrow ));

int32_t kstricmp(const char *pString1, const char *pString2) __attribute__ (( nothrow ));
int32_t kstrnicmp(const char *pString1, const char *pString2, size_t pLength) __attribute__ (( nothrow ));

char kto_lower(char pChar) __attribute__ (( nothrow ));
char kto_upper(char pChar) __attribute__ (( nothrow ));

char *kstrcat(char *pDest, const char *pSource) __attribute__ (( nothrow ));
char *kstrncat(char *pDest, const char *pSource, size_t pLength) __attribute__ (( nothrow ));

char *kstrchr(const char *pSource, char pChar) __attribute__ (( nothrow ));

void reverse(char str[], int length) __attribute__ (( nothrow ));
char* itoa(int64_t num, char* str, int base) __attribute__ (( nothrow ));
char* utoa(uint64_t num, char* str, int base) __attribute__ (( nothrow ));


#endif

