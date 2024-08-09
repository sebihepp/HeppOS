
#ifndef HEADER_PMM
#define HEADER_PMM

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "retvals.hpp"
#include "multiboot2.hpp"


#define PAGE_SIZE 	(4096)
#define MEM_MAX 	((uint32_t)2*1024*1024*1024) // 2GByte
#define BITMAP_SIZE	(MEM_MAX / PAGE_SIZE / 8)

class PMM {

private:

	static uint8_t Bitmap[BITMAP_SIZE];
	
	PMM();
	~PMM();
public:
	static retval_t Init(multiboot2_info_t *mbi);
	
};

#endif

