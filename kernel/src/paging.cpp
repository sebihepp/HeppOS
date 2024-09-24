
#include <paging.h>

static uint64_t GetCR3(void) {
	uint64_t _val = 0;
	asm volatile (
		"movq %%cr3, %0;\n"
		: "=r" (_val)
	);
	return _val;
}

void *Paging::GetPhysicalAddress(void *pVirtualAddress) {
	
	//ToDo: Implement
	return (void*)GetCR3();
}
