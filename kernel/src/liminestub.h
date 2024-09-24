
#ifndef HEADER_LIMINE
#define HEADER_LIMINE

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <limine.h>

#include <retvals.h>

class Limine {
private:
	Limine();
	~Limine();	
	
public:

	static retval_t Init(void);

	static limine_framebuffer_response *GetFramebufferResponse(void);
	static limine_memmap_response *GetMemoryMapResponse(void);
	static limine_hhdm_response *GetHHDMResponse(void);
	static limine_paging_mode_response *GetPagingModeResponse(void);
};


#endif
