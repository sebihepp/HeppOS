
#include <memory/paging.h>


void *CPaging::GetCR3(void) {
	
	void *_CR3 = NULL;
	asm volatile (
		"movq %%cr3, %%rax;\n"
		"movq %%rax, %0;\n"
		: "=m" (_CR3)
		: 
		: "rax"
	);
	
	return _CR3;
}

void *CPaging::GetPhysicalAddress(void *pVirtualAddress) {
	
	return NULL;
}

