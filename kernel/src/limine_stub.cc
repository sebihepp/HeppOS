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


#include <limine_stub.h>

__attribute__((used, section(".requests"))) static volatile uint64_t 
	limine_base_revision[] LIMINE_BASE_REVISION(4);
__attribute__((used, section(".requests"))) static volatile 
	limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
	.revision = 0,
	.response = NULL
};
__attribute__((used, section(".requests"))) static volatile 
limine_memmap_request memory_map_request = {
	.id = LIMINE_MEMMAP_REQUEST_ID,
	.revision = 0,
	.response = NULL
};
__attribute__((used, section(".requests"))) static volatile 
limine_hhdm_request hhdm_request = {
	.id = LIMINE_HHDM_REQUEST_ID,
	.revision = 0,
	.response = NULL
};
__attribute__((used, section(".requests"))) static volatile 
limine_paging_mode_request paging_mode_request = {
	.id = LIMINE_PAGING_MODE_REQUEST_ID,
	.revision = 1,
	.response = NULL,
	.mode = LIMINE_PAGING_MODE_X86_64_4LVL,
	.max_mode = LIMINE_PAGING_MODE_X86_64_5LVL,
	.min_mode = LIMINE_PAGING_MODE_X86_64_4LVL
};
__attribute__((used, section(".requests"))) static volatile 
limine_module_request module_request = {
	.id = LIMINE_MODULE_REQUEST_ID,
	.revision = 0,
	.response = NULL,
	.internal_module_count = 0,
	.internal_modules = NULL
};
__attribute__((used, section(".requests"))) static volatile 
limine_mp_request mp_request = {
	.id = LIMINE_MP_REQUEST_ID,
	.revision = 0,
	.response = NULL,
	.flags = 0
};
__attribute__((used, section(".requests"))) static volatile 
limine_date_at_boot_request date_at_boot_request = {
	.id = LIMINE_DATE_AT_BOOT_REQUEST_ID,
	.revision = 0,
	.response = NULL,
};
__attribute__((used, section(".requests"))) static volatile 
limine_executable_address_request executable_address_request = {
	.id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
	.revision = 0,
	.response = NULL,
};
__attribute__((used, section(".requests"))) static volatile 
limine_rsdp_request rsdp_request = {
	.id = LIMINE_RSDP_REQUEST_ID,
	.revision = 0,
	.response = NULL,
};
__attribute__((used, section(".requests"))) static volatile 
limine_executable_file_request executable_file_request = {
	.id = LIMINE_EXECUTABLE_FILE_REQUEST_ID,
	.revision = 0,
	.response = NULL,
};
__attribute__((used, section(".requests_start_marker"))) static volatile 
	uint64_t limine_start_marker[] LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".requests_end_marker"))) static volatile 
uint64_t limine_end_marker[] LIMINE_REQUESTS_END_MARKER;

static limine_module_response std_module_response = {
	.revision = 0,
	.module_count = 0,
	.modules = NULL
};

ReturnValue LimineStub::Init(void) {
	
	// Check Limine Revision
	if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
		return ReturnValueErrorLimineRevision;
	}
	
	// Check for NULL Pointers in responses
	if (GetFramebufferResponse() == NULL) {
		return ReturnValueErrorLimineNullPointer;
	}
	if (GetMemoryMapResponse() == NULL) {
		return ReturnValueErrorLimineNullPointer;
	}
	if (GetModuleResponse() == NULL) {
		module_request.response = &std_module_response;
	}
	if (GetPagingModeResponse() == NULL) {
		return ReturnValueErrorLimineNullPointer;
	}	
	if (GetMPResponse() == NULL) {
		return ReturnValueErrorLimineNullPointer;
	}	
	if (GetExecutableAddressResponse() == NULL) {
		return ReturnValueErrorLimineNullPointer;
	}	
	if (GetExecutableFileResponse() == NULL) {
		return ReturnValueErrorLimineNullPointer;
	}	
	if (GetDateAtBootResponse() == NULL) {
		return ReturnValueErrorLimineNullPointer;
	}	
	if (GetRSDPResponse() == NULL) {
		return ReturnValueErrorLimineNullPointer;
	}	
	
	return ReturnValueOk;
}

limine_framebuffer_response *LimineStub::GetFramebufferResponse(void) {
	return framebuffer_request.response;
}

limine_memmap_response *LimineStub::GetMemoryMapResponse(void) {
	return memory_map_request.response;
}

limine_hhdm_response *LimineStub::GetHHDMResponse(void) {
	return hhdm_request.response;
}

limine_paging_mode_response *LimineStub::GetPagingModeResponse(void) {
	return paging_mode_request.response;
}

limine_module_response *LimineStub::GetModuleResponse(void) {
	return module_request.response;
}

limine_mp_response *LimineStub::GetMPResponse(void) {
	return mp_request.response;
}

limine_rsdp_response *LimineStub::GetRSDPResponse(void) {
	return rsdp_request.response;
}

limine_executable_address_response *LimineStub::GetExecutableAddressResponse(void) {
	return executable_address_request.response;
}

limine_executable_file_response *LimineStub::GetExecutableFileResponse(void) {
	return executable_file_request.response;
}

limine_date_at_boot_response *LimineStub::GetDateAtBootResponse(void) {
	return date_at_boot_request.response;
}
