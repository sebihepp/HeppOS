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

#include <return_value.h>
	
const char *GetReturnValueString(const ReturnValue return_value) {
	
	switch (return_value) {
		
		case ReturnValueOk:
			return "OK";
		case ReturnValueErrorGeneral:
			return "GENERAL_ERROR";
		case ReturnValueErrorTest:
			return "TEST_ERROR";
		case ReturnValueErrorLimineRevision:
			return "LIMINE_REVISION_ERROR";
		case ReturnValueErrorNoFramebuffer:
			return "NO_FRAMEBUFFER_ERROR";
		case ReturnValueErrorUnsupportedVideoMode:
			return "UNSUPPORTED_VIDEO_MODE_ERROR";
		case ReturnValueErrorNoFXSave:
			return "NO_FXSAVE_ERROR";
		case ReturnValueErrorNo1GBPages:
			return "NO_1GB_PAGES_ERROR";
		case ReturnValueErrorNoGlobalPages:
			return "NO_GLOBAL_PAGES_ERROR";
		case ReturnValueErrorCPUIDLevel:
			return "CPUID_LEVEL_ERROR";
		case ReturnValueErrorCPUIDExtLevel:
			return "CPUID_EXT_LEVEL_ERROR";
		case ReturnValueErrorNoPAT:
			return "NO_PAT_ERROR";
		case ReturnValueErrorNoHHDM:
			return "NO_HHDM_ERROR";
		case ReturnValueErrorLiminePagingMode:
			return "LIMINE_PAGING_MODE_ERROR";
		case ReturnValueErrorLimineNullPointer:
			return "LIMINE_NULL_POINTER_ERROR";
		case ReturnValueErrorPageNotFound:
			return "PAGE_NOT_FOUND_ERROR";
		case ReturnValueErrorPageNotPresent:
			return "PAGE_NOT_PRESENT_ERROR";
		case ReturnValueErrorInvalidPageLevel:
			return "INVALID_PAGE_LEVEL_ERROR";			
		case ReturnValueErrorOutOfMemoryPhysical:
			return "OUT_OF_MEMORY_PHYSICAL_ERROR";
		case ReturnValueErrorOutOfMemoryVirtual:
			return "OUT_OF_MEMORY_VIRTUAL_ERROR";

		case ReturnValueWarningTest:
			return "TEST_WARNING";
		case ReturnValueWarningGeneral:
			return "GENERAL_WARNING";

		default:
			break;
	}
	return "ReturnValueUnknown";
}
