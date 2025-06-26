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

#ifndef HEADER_IO
#define HEADER_IO

#include <stddef.h>
#include <stdint.h>

inline uint8_t inb(uint16_t pPort) __attribute__(( nothrow, always_inline ));
inline uint8_t inb(uint16_t pPort)
{
	uint8_t _value = 0;
	asm volatile (
		"mov %1, %%dx;\n"
		"inb %%dx, %%al;\n"
		: "=a" (_value)
		: "m" (pPort)
		: "dx"
	);
	return _value;
}

inline uint16_t inw(uint16_t pPort) __attribute__(( nothrow, always_inline ));
inline uint16_t inw(uint16_t pPort)
{
	uint16_t _value = 0;
	asm volatile (
		"mov %1, %%dx;\n"
		"inw %%dx, %%ax;\n"
		: "=a" (_value)
		: "m" (pPort)
		: "dx"
	);
	return _value;
}

inline uint32_t inl(uint16_t pPort) __attribute__(( nothrow, always_inline ));
inline uint32_t inl(uint16_t pPort)
{
	uint32_t _value = 0;
	asm volatile (
		"mov %1, %%dx;\n"
		"inl %%dx, %%eax;\n"
		: "=a" (_value)
		: "m" (pPort)
		: "dx"
	);
	return _value;	
}


inline void outb(uint16_t pPort, uint8_t pValue) __attribute__(( nothrow, always_inline ));
inline void outb(uint16_t pPort, uint8_t pValue)
{
	asm volatile (
		"mov %0, %%al;\n"
		"mov %1, %%dx;\n"
		"outb %%al, %%dx;\n"
		:
		: "m" (pValue), "m" (pPort)
		: "al", "dx"
	);
}

inline void outw(uint16_t pPort, uint16_t pValue) __attribute__(( nothrow, always_inline ));
inline void outw(uint16_t pPort, uint16_t pValue)
{
	asm volatile (
		"mov %0, %%ax;\n"
		"mov %1, %%dx;\n"
		"outw %%ax, %%dx;\n"
		:
		: "m" (pValue), "m" (pPort)
		: "ax", "dx"
	);
}

inline void outl(uint16_t pPort, uint32_t pValue) __attribute__(( nothrow, always_inline ));
inline void outl(uint16_t pPort, uint32_t pValue)
{
	asm volatile (
		"mov %0, %%eax;\n"
		"mov %1, %%dx;\n"
		"outl %%eax, %%dx;\n"
		:
		: "m" (pValue), "m" (pPort)
		: "eax", "dx"
	);
}

#endif

