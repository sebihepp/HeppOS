
#ifndef CXXSTUB_HEADER
#define CXXSTUB_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void *memchr(const void *pSource, int8_t pValue, size_t pSize) __attribute__(( nothrow ));
int32_t memcmp(const void* pData1, const void* pData2, size_t pLength) __attribute__(( nothrow ));
void* memcpy(void *pDest, const void *pSource, size_t pLength) __attribute__(( nothrow ));
void* memmove(void *pDest, const void *pSource, size_t pLength) __attribute__(( nothrow ));
void* memset(void *pDest, int8_t pValue, size_t pLength) __attribute__(( nothrow ));

#ifdef __cplusplus
}
#endif

#endif

