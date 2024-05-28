

#ifndef PMM_HEADER
#define PMM_HEADER

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


typedef uint32_t pmm_bitmap_type;
#define PMM_PAGE_SIZE 4096
#define PMM_BITMAP_LOW_SIZE ((1024*1024) / PMM_PAGE_SIZE / (sizeof(pmm_bitmap_type) * 8)) //1MB

class PMM {

private:
	static pmm_bitmap_type bitmapLow[PMM_BITMAP_LOW_SIZE]; 


public:
	PMM() = delete;
	
	static uint32_t init(void *mbi);
	
	static uint32_t allocPage(void* &address);
	static uint32_t allocPages(uint32_t numPages, void* &address);
	static uint32_t allocLowPage(void* &address);
	static uint32_t allocLowPages(uint32_t numPages, void* &address);
	
	static uint32_t freePage(void *address);
	static uint32_t freePages(void *address, uint32_t numPages);
};

#endif
