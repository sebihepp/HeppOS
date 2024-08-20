
#ifndef HEADER_PMM
#define HEADER_PMM

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "retvals.hpp"
#include "multiboot2.hpp"


#define PMM_BITMAP_SIZE	(16*1024)	// one bit per page for the first 2GiB

class PMM {

private:

	static uint32_t Bitmap[PMM_BITMAP_SIZE];
	
	static void SetUsed(void *Address);
	static void SetUsedRange(void *Address, size_t size);
	static void SetFree(void *Address);
	static void SetFreeRange(void *Address, size_t size);
	
	PMM();
	~PMM();
	
public:
	static retval_t Init(multiboot2_info_t *mbi);
	
	static void *Allocate(void);
	static void Free(void *pAddress);
	
};

#endif

