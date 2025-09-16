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


#pragma once

#include <ktype.h>

#define LIMINE_API_REVISION 3
#include <limine.h>

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
	static limine_executable_address_response *GetExecutableAddressResponse(void) __attribute(( nothrow ));
	static limine_executable_file_response *GetExecutableFileResponse(void) __attribute(( nothrow ));
	static limine_mp_response *GetMPResponse(void) __attribute(( nothrow ));
    static limine_rsdp_response *GetRSDPResponse(void) __attribute(( nothrow ));
    static limine_date_at_boot_response *GetDateAtBootResponse(void) __attribute(( nothrow ));

	
};

