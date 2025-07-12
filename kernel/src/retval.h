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

#ifndef RETVAL_HEADER
#define RETVAL_HEADER

enum ReturnValue_t {
	RETVAL_OK = 0,					
	RETVAL_ERROR_GENERAL,
	RETVAL_ERROR_LIMINE_REV,
	RETVAL_ERROR_NO_FRAMEBUFFER,
	RETVAL_ERROR_TEST,	
	RETVAL_ERROR_VIDEOMODE,
	RETVAL_ERROR_NO_FXSAVE,
	RETVAL_ERROR_NO_1GB_PAGES,
	RETVAL_ERROR_NO_GLOBAL_PAGES,
	RETVAL_ERROR_CPUID_LEVEL,
	RETVAL_ERROR_CPUID_EXT_LEVEL,
	RETVAL_ERROR_NO_PAT,
	RETVAL_ERROR_NO_HHDM,
	RETVAL_ERROR_LIMINE_PAGING_MODE,
	RETVAL_ERROR_LIMINE_NULL_POINTER,
	RETVAL_ERROR_PAGE_NOT_FOUND,
	RETVAL_ERROR_PAGE_NOT_PRESENT,
	RETVAL_ERROR_INVALID_PAGELEVEL,
	RETVAL_ERROR_OOM_PHYSICAL,
	RETVAL_ERROR_OOM_VIRTUAL,
};

#define IS_ERROR(x) ((x) != RETVAL_OK)
#define IS_SUCCESS(x) ((x) == RETVAL_OK)

const char *GetReturnValueString(const ReturnValue_t pReturnValue) __attribute__((const, nothrow));

#endif

