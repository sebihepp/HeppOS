
#ifndef HEADER_PMM
#define HEADER_PMM

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "retvals.hpp"
#include "multiboot2.hpp"


#define PMM_PAGE_SIZE 	(4096)
#define PMM_MEM_MAX 	((uint32_t)2*1024*1024*1024) // 2GByte
#define PMM_BITMAP_SIZE	(PMM_MEM_MAX / PMM_PAGE_SIZE / 8)

class PMM {

private:

	static uint8_t Bitmap[PMM_BITMAP_SIZE];
	
	PMM();
	~PMM();
public:
	static retval_t Init(multiboot2_info_t *mbi);
	
};

#endif

