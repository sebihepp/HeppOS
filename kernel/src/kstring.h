
#ifndef HEADER_KSTRING
#define HEADER_KSTRING

#include <stddef.h>
#include <stdint.h>


size_t kstrlen(const char *pString);

char *kstrcpy(const char *pSource, char *pDest);
char *kstrncpy(const char *pSource, char *pDest, size_t pLength);

int32_t kstrcmp(const char *pString1, const char *pString2);
int32_t kstrncmp(const char *pString1, const char *pString2, size_t pLength);

#endif

