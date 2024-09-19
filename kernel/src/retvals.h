
#ifndef RETVALS_HEADER
#define RETVALS_HEADER

#include <stddef.h>
#include <stdint.h>

typedef uint32_t retval_t;

#define RETVAL_OK 						0
#define RETVAL_ERROR_GENERAL			1
#define RETVAL_ERROR_LIMINE_REV			2
#define RETVAL_ERROR_NO_FRAMEBUFFER		3
#define RETVAL_ERROR_TEST				4
#define RETVAL_ERROR_VIDEOMODE			5
#define RETVAL_ERROR_NO_FXSAVE			6
#define RETVAL_ERROR_NO_1GB_PAGES		7
#define RETVAL_ERROR_NO_GLOBAL_PAGES	8
#define RETVAL_ERROR_CPUID_LEVEL		9
#define RETVAL_ERROR_CPUID_EXT_LEVEL	10
#define RETVAL_ERROR_NO_PAT				11


#endif
