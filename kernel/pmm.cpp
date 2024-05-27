
#include "retvals.h"
#include "pmm.hpp"

uint32_t PMM::bitmapLow[PMM_BITMAP_LOW_SIZE];

uint32_t PMM::init(void *mbi) {
	
	//Set all pages in use and later free only available pages
	for (uint32_t i = 0; i < PMM_BITMAP_LOW_SIZE; i++) {
		bitmapLow[i] = 1; 
	}
	
	return RETVAL_ERROR;
}

uint32_t PMM::allocPage(void* &address) {
	
	address = (void*)NULL;
	return RETVAL_ERROR;
}

uint32_t PMM::allocPages(uint32_t numPages, void* &address) {

	address = (void*)NULL;
	return RETVAL_ERROR;
}
	
uint32_t PMM::allocLowPage(void* &address) {
	
	address = (void*)NULL;
	return RETVAL_ERROR;
}

uint32_t PMM::allocLowPages(uint32_t numPages, void* &address) {

	address = (void*)NULL;
	return RETVAL_ERROR;
}

uint32_t PMM::freePage(void *address) {
	
	return RETVAL_ERROR;
}

uint32_t PMM::freePages(void *address, uint32_t numPages) {
	
	return RETVAL_ERROR;
}

