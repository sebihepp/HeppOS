
#ifndef HEADER_PAGING
#define HEADER_PAGING

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <limine.h>

#include <retvals.h>


class Paging {
private:
	Paging();
	~Paging();


public:
	static void *GetPhysicalAddress(void *pVirtualAddress);
	


};

#endif
