
#ifndef HEADER_LIMINE
#define HEADER_LIMINE

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <limine.h>

#include <retvals.h>

class CLimine {
private:
	CLimine();
	~CLimine();	
	
public:

	static ReturnValue_t Init(void);

	static limine_framebuffer_response *GetFramebufferResponse(void);
	static limine_memmap_response *GetMemoryMapResponse(void);
	static limine_hhdm_response *GetHHDMResponse(void);
	static limine_paging_mode_response *GetPagingModeResponse(void);
	static limine_module_response *GetModuleResponse(void);
	static limine_kernel_address_response *GetKernelAddressResponse(void);
	static limine_kernel_file_response *GetKernelFileResponse(void);
	
};


#endif
