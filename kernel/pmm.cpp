
#include "retvals.h"
#include "pmm.hpp"

uint32_t PMM::init(void *mbi) {
	
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
	
uint32_t PMM::freePage(void *address) {
	
	return RETVAL_ERROR;
}

uint32_t PMM::freePages(void *address, uint32_t numPages) {
	
	return RETVAL_ERROR;
}

