
#ifndef CXXSTUB_HEADER
#define CXXSTUB_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t memcmp(const void*, const void*, size_t) __attribute__(( nothrow ));
void* memcpy(void*, const void*, size_t) __attribute__(( nothrow ));
void* memmove(void*, const void*, size_t) __attribute__(( nothrow ));
void* memset(void*, int, size_t) __attribute__(( nothrow ));

#ifdef __cplusplus
}
#endif

#endif

