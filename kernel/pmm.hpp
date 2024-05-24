
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef PMM_HEADER
#define PMM_HEADER

class PMM {

private:



public:
	PMM() = delete;
	
	static uint32_t init(void *mbi);
	
	static uint32_t allocPage(void* &address);
	static uint32_t allocPages(uint32_t numPages, void* &address);
	
	static uint32_t freePage(void *address);
	static uint32_t freePages(void *address, uint32_t numPages);
	
};

#endif
