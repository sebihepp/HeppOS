
#include <liminestub.h>

__attribute__((used, section(".requests"))) static volatile LIMINE_BASE_REVISION(2);
__attribute__((used, section(".requests"))) static volatile limine_framebuffer_request FramebufferRequest = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0,
	.response = NULL
};
__attribute__((used, section(".requests"))) static volatile limine_memmap_request MemoryMapRequest = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0,
	.response = NULL
};
__attribute__((used, section(".requests"))) static volatile limine_hhdm_request HHDMRequest = {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0,
	.response = NULL
};
__attribute__((used, section(".requests"))) static volatile limine_paging_mode_request PagingModeRequest = {
	.id = LIMINE_PAGING_MODE_REQUEST,
	.revision = 1,
	.response = NULL,
	.mode = LIMINE_PAGING_MODE_X86_64_4LVL,
	.max_mode = LIMINE_PAGING_MODE_X86_64_4LVL,
	.min_mode = LIMINE_PAGING_MODE_X86_64_4LVL
};
__attribute__((used, section(".requests_start_marker"))) static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".requests_end_marker"))) static volatile LIMINE_REQUESTS_END_MARKER;


retval_t Limine::Init(void) {
	if (LIMINE_BASE_REVISION_SUPPORTED == false) {
		return RETVAL_ERROR_LIMINE_REV;
	}
	return RETVAL_OK;
}

limine_framebuffer_response *Limine::GetFramebufferResponse(void) {
	return FramebufferRequest.response;
}

limine_memmap_response *Limine::GetMemoryMapResponse(void) {
	return MemoryMapRequest.response;
}

limine_hhdm_response *Limine::GetHHDMResponse(void) {
	return HHDMRequest.response;
}

limine_paging_mode_response *Limine::GetPagingModeResponse(void) {
	return PagingModeRequest.response;
}
