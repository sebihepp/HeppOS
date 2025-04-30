
#ifndef HEADER_PMM
#define HEADER_PMM

#include <retval.h>
#include <memory/paging.h>
#include <liminestub.h>

class CPMM {
private:
	
	CPMM() = delete;
	~CPMM() = delete;
public:

	static ReturnValue_t PreInit(void) __attribute__ (( nothrow ));
	static ReturnValue_t Init(void) __attribute__ (( nothrow ));
	
};

#endif
