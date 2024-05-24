
#include "pmm.hpp"

uint32_t PMM::init(void *mbi) {
	
	return 1;
}

uint32_t PMM::allocPage(void* &address) {
	
	address = (void*)NULL;
	return 1;
}

uint32_t PMM::allocPages(uint32_t numPages, void* &address) {

	address = (void*)NULL;
	return 1;
}
	
uint32_t PMM::freePage(void *address) {
	
	return 1;
}

uint32_t PMM::freePages(void *address, uint32_t numPages) {
	
	return 1;
}

