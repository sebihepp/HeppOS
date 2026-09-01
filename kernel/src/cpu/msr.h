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

#include <stddef.h>
#include <stdint.h>

#define MSR_PAT (0x277)
#define MSR_EFER (0xC0000080)

inline void ReadMSR(uint32_t msr, uint32_t *low, uint32_t *high) 
	__attribute__(( nothrow, always_inline ));
inline void ReadMSR(uint32_t msr, uint32_t *low, uint32_t *high)
{
	asm volatile 
	(
		"mov %2, %%ecx;\n"
		"rdmsr;\n"
		: "=a" (*low), "=d" (*high)
		: "m" (msr)
		: "ecx"
	);

}

inline void ReadMSR(uint32_t msr, uint64_t *data) 
	__attribute__(( nothrow, always_inline ));
inline void ReadMSR(uint32_t msr, uint64_t *data)
{
	uint32_t low = 0;
	uint32_t high = 0;
	ReadMSR(msr, &low, &high);
	*data = static_cast<uint64_t>(low);
	*data |= static_cast<uint64_t>(high) << 32;
}

inline void WriteMSR(uint32_t msr, uint32_t low, uint32_t high) 
	__attribute__(( nothrow, always_inline ));
inline void WriteMSR(uint32_t msr, uint32_t low, uint32_t high)
{
	asm volatile 
	(
		"mov %0, %%eax;\n"
		"mov %1, %%edx;\n"
		"mov %2, %%ecx;\n"
		"wrmsr;\n"
		:
		: "m" (low), "m" (high), "m" (msr)
		: "eax", "ecx", "edx"
	);	
}

inline void WriteMSR(uint32_t msr, uint64_t *data) 
	__attribute__(( nothrow, always_inline ));
inline void WriteMSR(uint32_t msr, uint64_t *data)
{
	uint32_t low = *data & 0xFFFFFFFF;
	uint32_t high = (*data >> 32) & 0xFFFFFFFF;
	WriteMSR(msr, low, high);
}


