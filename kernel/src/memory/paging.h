
#ifndef HEADER_PAGING
#define HEADER_PAGING

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <limine.h>

#include <retvals.h>


class CPaging {
private:
	CPaging();
	~CPaging();


public:

	static void *GetCR3(void);
	static void *GetPhysicalAddress(void *pVirtualAddress);
	


};

#endif
