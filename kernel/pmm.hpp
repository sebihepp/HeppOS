

#ifndef PMM_HEADER
#define PMM_HEADER

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PMM_BITMAP_LOW_SIZE 8 //8x32bit = 256 Pages x 4K = 1MByte Memory

class PMM {

private:
	static uint32_t bitmapLow[PMM_BITMAP_LOW_SIZE]; 


public:
	PMM() = delete;
	
	static uint32_t init(void *mbi);
	
	static uint32_t allocPage(void* &address);
	static uint32_t allocPages(uint32_t numPages, void* &address);
	
	static uint32_t freePage(void *address);
	static uint32_t freePages(void *address, uint32_t numPages);
	
};

#endif
