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

#include <retval.h>
	
const char *GetReturnValueString(const ReturnValue_t pReturnValue) {
	
	switch (pReturnValue) {
		
		case RETVAL_OK:
			return "OK";
		case RETVAL_ERROR_GENERAL:
			return "ERROR_GENERAL";
		case RETVAL_ERROR_LIMINE_REV:
			return "ERROR_LIMINE_REV";
		case RETVAL_ERROR_NO_FRAMEBUFFER:
			return "ERROR_NO_FRAMEBUFFER";
		case RETVAL_ERROR_TEST:
			return "ERROR_TEST";
		case RETVAL_ERROR_VIDEOMODE:
			return "ERROR_VIDEOMODE";
		case RETVAL_ERROR_NO_FXSAVE:
			return "ERROR_NO_FXSAVE";
		case RETVAL_ERROR_NO_1GB_PAGES:
			return "ERROR_NO_1G_PAGES";
		case RETVAL_ERROR_NO_GLOBAL_PAGES:
			return "ERROR_NO_GLOBAL_PAGES";
		case RETVAL_ERROR_CPUID_LEVEL:
			return "ERROR_CPUID_LEVEL";
		case RETVAL_ERROR_CPUID_EXT_LEVEL:
			return "ERROR_CPUID_EXT_LEVEL";
		case RETVAL_ERROR_NO_PAT:
			return "ERROR_NO_PAT";
		case RETVAL_ERROR_NO_HHDM:
			return "ERROR_NO_HHDM";
		case RETVAL_ERROR_LIMINE_PAGING_MODE:
			return "ERROR_LIMINE_PAGING_MODE";
		case RETVAL_ERROR_LIMINE_NULL_POINTER:
			return "ERROR_LIMINE_NULL_POINTER";
		case RETVAL_ERROR_PAGE_NOT_FOUND:
			return "ERROR_PAGE_NOT_FOUND";
		case RETVAL_ERROR_PAGE_NOT_PRESENT:
			return "ERROR_PAGE_NOT_PRESENT";
		case RETVAL_ERROR_INVALID_PAGELEVEL:
			return "ERROR_INVALID_PAGELEVEL";
		default:
			break;
	}
	return "ERROR_UNKNOWN";
}
