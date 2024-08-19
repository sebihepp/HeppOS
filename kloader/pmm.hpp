
#ifndef HEADER_PMM
#define HEADER_PMM

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "retvals.hpp"
#include "multiboot2.hpp"


#define PMM_BITMAP_SIZE	(64*1024)	// one bit per page for the first 2GiB

class PMM {

private:

	static uint8_t Bitmap[PMM_BITMAP_SIZE];
	
	PMM();
	~PMM();
public:
	static retval_t Init(multiboot2_info_t *mbi);
	
	static void *Allocate(void);
	static void Free(void *pAddress);
	
};

#endif

