
#ifndef HEADER_KSTRING
#define HEADER_KSTRING

#include <stddef.h>
#include <stdint.h>


size_t kstrlen(const char *pString) __attribute__ (( nothrow ));

char *kstrncpy(char *pDest, const char *pSource, size_t pLength) __attribute__ (( nothrow ));
static inline char *kstrcpy(char *pDest, const char *pSource) __attribute__ (( nothrow , always_inline ));
static inline char *kstrcpy(char *pDest, const char *pSource) {
	return kstrncpy(pDest, pSource, -1);
}


int32_t kstrncmp(const char *pString1, const char *pString2, size_t pLength) __attribute__ (( nothrow ));
static inline int32_t kstrcmp(const char *pString1, const char *pString2) __attribute__ (( nothrow , always_inline ));
static inline int32_t kstrcmp(const char *pString1, const char *pString2) {
	return kstrncmp(pString1, pString2, -1);
}


int32_t kstrnicmp(const char *pString1, const char *pString2, size_t pLength) __attribute__ (( nothrow ));
static inline int32_t kstricmp(const char *pString1, const char *pString2) __attribute__ (( nothrow , always_inline ));
static inline int32_t kstricmp(const char *pString1, const char *pString2) {
	return kstrnicmp(pString1, pString2, -1);
}


char *kstrncat(char *pDest, const char *pSource, size_t pLength) __attribute__ (( nothrow ));
static inline char *kstrcat(char *pDest, const char *pSource) __attribute__ (( nothrow , always_inline));
static inline char *kstrcat(char *pDest, const char *pSource) {
	return kstrncat(pDest, pSource, -1);
}


char *kstrchr(const char *pSource, char pChar) __attribute__ (( nothrow ));

char kto_lower(char pChar) __attribute__ (( nothrow ));
char kto_upper(char pChar) __attribute__ (( nothrow ));

void reverse(char str[], int length) __attribute__ (( nothrow ));
char* itoa(int64_t num, char* str, int base) __attribute__ (( nothrow ));
char* utoa(uint64_t num, char* str, int base) __attribute__ (( nothrow ));


#endif

