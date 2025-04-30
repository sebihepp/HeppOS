
#ifndef HEADER_LIMINE
#define HEADER_LIMINE

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <limine.h>

#include <retval.h>

class CLimine {
private:
	CLimine() = delete;
	~CLimine() = delete;	
	
public:

	static ReturnValue_t Init(void) __attribute(( nothrow ));

	static limine_framebuffer_response *GetFramebufferResponse(void) __attribute(( nothrow ));
	static limine_memmap_response *GetMemoryMapResponse(void) __attribute(( nothrow ));
	static limine_hhdm_response *GetHHDMResponse(void) __attribute(( nothrow ));
	static limine_paging_mode_response *GetPagingModeResponse(void) __attribute(( nothrow ));
	static limine_module_response *GetModuleResponse(void) __attribute(( nothrow ));
	static limine_kernel_address_response *GetKernelAddressResponse(void) __attribute(( nothrow ));
	static limine_kernel_file_response *GetKernelFileResponse(void) __attribute(( nothrow ));
	
};


#endif

