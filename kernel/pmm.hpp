

#ifndef PMM_HEADER
#define PMM_HEADER

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot2.hpp"

typedef uint32_t pmm_bitmap_t;
#define PMM_PAGE_SIZE 4096
#define PMM_HIGH_START (16*1024*1024)
#define PMM_BITMAP_LOW_SIZE (PMM_HIGH_START / PMM_PAGE_SIZE / (sizeof(pmm_bitmap_t) * 8)) 
#define PMM_BITMAP_HIGH_SIZE (((4*(uint64_t)1024*1024*1024) - PMM_HIGH_START) / PMM_PAGE_SIZE / (sizeof(pmm_bitmap_t) * 8))

class PMM {

private:
	static pmm_bitmap_t bitmapLow[PMM_BITMAP_LOW_SIZE]; 
	static pmm_bitmap_t bitmapHigh[PMM_BITMAP_HIGH_SIZE]; 
	
	static uint32_t setBitmapValue(void *address, uint8_t value);
	
public:
	PMM() = delete;
	
	static uint32_t init(multiboot2_info_t *mbi);
	
	static uint32_t allocPage(void* &address);
	static uint32_t allocPages(uint32_t numPages, void* &address);
	static uint32_t allocLowPage(void* &address);
	static uint32_t allocLowPages(uint32_t numPages, void* &address);
	
	static uint32_t freePage(void *address);
	static uint32_t freePages(void *address, uint32_t numPages);
};

#endif
