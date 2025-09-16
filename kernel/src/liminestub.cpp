/* 
    Copyright (C) 2025  Sebastian Hepp aka sebihepp

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include <liminestub.h>

__attribute__((used, section(".requests"))) static volatile LIMINE_BASE_REVISION(3);
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
	.max_mode = LIMINE_PAGING_MODE_X86_64_5LVL,
	.min_mode = LIMINE_PAGING_MODE_X86_64_4LVL
};
__attribute__((used, section(".requests"))) static volatile limine_module_request ModuleRequest = {
	.id = LIMINE_MODULE_REQUEST,
	.revision = 0,
	.response = NULL,
	.internal_module_count = 0,
	.internal_modules = NULL
};
__attribute__((used, section(".requests"))) static volatile limine_mp_request MPRequest = {
	.id = LIMINE_MP_REQUEST,
	.revision = 0,
	.response = NULL,
	.flags = 0
};
__attribute__((used, section(".requests"))) static volatile limine_date_at_boot_request DateAtBootRequest = {
	.id = LIMINE_DATE_AT_BOOT_REQUEST,
	.revision = 0,
	.response = NULL,
};
__attribute__((used, section(".requests"))) static volatile limine_executable_address_request ExecutableAddressRequest = {
	.id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
	.revision = 0,
	.response = NULL,
};
__attribute__((used, section(".requests"))) static volatile limine_rsdp_request RSDPRequest = {
	.id = LIMINE_RSDP_REQUEST,
	.revision = 0,
	.response = NULL,
};
__attribute__((used, section(".requests"))) static volatile limine_executable_file_request ExecutableFileRequest = {
	.id = LIMINE_EXECUTABLE_FILE_REQUEST,
	.revision = 0,
	.response = NULL,
};
__attribute__((used, section(".requests_start_marker"))) static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".requests_end_marker"))) static volatile LIMINE_REQUESTS_END_MARKER;

static limine_module_response gSTDModuleResponse = {
	.revision = 0,
	.module_count = 0,
	.modules = NULL
};

ReturnValue_t CLimine::Init(void) {
	
	// Check Limine Revision
	if (LIMINE_BASE_REVISION_SUPPORTED == false) {
		return RETVAL_ERROR_LIMINE_REV;
	}
	
	// Check for NULL Pointers in responses
	if (GetFramebufferResponse() == NULL) {
		return RETVAL_ERROR_LIMINE_NULL_POINTER;
	}
	if (GetMemoryMapResponse() == NULL) {
		return RETVAL_ERROR_LIMINE_NULL_POINTER;
	}
	if (GetModuleResponse() == NULL) {
		ModuleRequest.response = &gSTDModuleResponse;
	}
	if (GetPagingModeResponse() == NULL) {
		return RETVAL_ERROR_LIMINE_NULL_POINTER;
	}	
	if (GetMPResponse() == NULL) {
		return RETVAL_ERROR_LIMINE_NULL_POINTER;
	}	
	if (GetExecutableAddressResponse() == NULL) {
		return RETVAL_ERROR_LIMINE_NULL_POINTER;
	}	
	if (GetExecutableFileResponse() == NULL) {
		return RETVAL_ERROR_LIMINE_NULL_POINTER;
	}	
	if (GetDateAtBootResponse() == NULL) {
		return RETVAL_ERROR_LIMINE_NULL_POINTER;
	}	
	if (GetRSDPResponse() == NULL) {
		return RETVAL_ERROR_LIMINE_NULL_POINTER;
	}	
	
	return RETVAL_OK;
}

limine_framebuffer_response *CLimine::GetFramebufferResponse(void) {
	return FramebufferRequest.response;
}

limine_memmap_response *CLimine::GetMemoryMapResponse(void) {
	return MemoryMapRequest.response;
}

limine_hhdm_response *CLimine::GetHHDMResponse(void) {
	return HHDMRequest.response;
}

limine_paging_mode_response *CLimine::GetPagingModeResponse(void) {
	return PagingModeRequest.response;
}

limine_module_response *CLimine::GetModuleResponse(void) {
	return ModuleRequest.response;
}

limine_mp_response *CLimine::GetMPResponse(void) {
	return MPRequest.response;
}

limine_rsdp_response *CLimine::GetRSDPResponse(void) {
	return RSDPRequest.response;
}

limine_executable_address_response *CLimine::GetExecutableAddressResponse(void) {
	return ExecutableAddressRequest.response;
}

limine_executable_file_response *CLimine::GetExecutableFileResponse(void) {
	return ExecutableFileRequest.response;
}

limine_date_at_boot_response *CLimine::GetDateAtBootResponse(void) {
	return DateAtBootRequest.response;
}