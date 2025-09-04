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

#ifndef CPUID_HEADER
#define CPUID_HEADER

#include <ktype.h>

struct cpuid_retval_t
{
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
} __attribute__((packed));

static inline void cpuid(const uint32_t pCode, cpuid_retval_t &pRetval)  __attribute__ (( nothrow, always_inline ));
static inline void cpuid(const uint32_t pCode, cpuid_retval_t &pRetval)
{
	asm volatile 
	(
		"cpuid" :
		"=a" (pRetval.eax), "=b" (pRetval.ebx), "=c" (pRetval.ecx), "=d" (pRetval.edx) :
		"a" (pCode) :
	);	
}

#endif

