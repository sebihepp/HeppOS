
#ifndef RETVALS_HEADER
#define RETVALS_HEADER

#include <stddef.h>
#include <stdint.h>

typedef uint32_t retval_t;

#define RETVAL_OK 						0
#define RETVAL_ERROR_GENERAL			1
#define RETVAL_ERROR_MB2_MAGIC			2
#define RETVAL_ERROR_NO_LONGMODE		3
#define RETVAL_ERROR_NO_FRAMEBUFFER		4
#define RETVAL_ERROR_TEST				5
#define RETVAL_ERROR_VIDEOMODE			6
#define RETVAL_ERROR_NO_PAE				7
#define RETVAL_ERROR_NO_SSE				8
#define RETVAL_ERROR_NO_SSE2			9
#define RETVAL_ERROR_NO_FXSAVE			10
#define RETVAL_ERROR_NO_1GB_PAGES		11

#endif