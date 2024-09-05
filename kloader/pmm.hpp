
#ifndef HEADER_PMM
#define HEADER_PMM

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "retvals.hpp"
#include "multiboot2.hpp"

#define PMM_MAX_ADDRESS (2*1024*1024*1024)
#define PMM_BITMAP_SIZE	(16*1024)	// one bit per page for the first 2GiB

class PMM {

private:

	static uint32_t Bitmap[PMM_BITMAP_SIZE];
	
	static void SetUsed(const void * const pAddress);
	static void SetUsedRange(const void * const pAddress, const size_t pSize);
	static void SetFree(const void * const Address);
	static void SetFreeRange(const void * const pAddress, const size_t pSize);
	
	PMM();
	~PMM();
	
public:
	static retval_t Init(const multiboot2_info_t *pMBInfo);
	
	static void *Allocate(void);
	static void Free(const void * const pAddress);
	
};

#endif

