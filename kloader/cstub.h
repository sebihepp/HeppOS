
#ifndef CXXSTUB_HEADER
#define CXXSTUB_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t memcmp(const void*, const void*, size_t);
void* memcpy(void*, const void*, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);

#ifdef __cplusplus
}
#endif

#endif
