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

inline uint8_t inb(uint16_t port) __attribute__(( nothrow, always_inline ));
inline uint8_t inb(uint16_t port)
{
	uint8_t value = 0;
	asm volatile (
		"mov %1, %%dx;\n"
		"inb %%dx, %%al;\n"
		: "=a" (value)
		: "m" (port)
		: "dx"
	);
	return value;
}

inline uint16_t inw(uint16_t port) __attribute__(( nothrow, always_inline ));
inline uint16_t inw(uint16_t port)
{
	uint16_t value = 0;
	asm volatile (
		"mov %1, %%dx;\n"
		"inw %%dx, %%ax;\n"
		: "=a" (value)
		: "m" (port)
		: "dx"
	);
	return value;
}

inline uint32_t inl(uint16_t port) __attribute__(( nothrow, always_inline ));
inline uint32_t inl(uint16_t port)
{
	uint32_t value = 0;
	asm volatile (
		"mov %1, %%dx;\n"
		"inl %%dx, %%eax;\n"
		: "=a" (value)
		: "m" (port)
		: "dx"
	);
	return value;	
}


inline void outb(uint16_t port, uint8_t value) __attribute__(( nothrow, always_inline ));
inline void outb(uint16_t port, uint8_t value)
{
	asm volatile (
		"mov %0, %%al;\n"
		"mov %1, %%dx;\n"
		"outb %%al, %%dx;\n"
		:
		: "m" (value), "m" (port)
		: "al", "dx"
	);
}

inline void outw(uint16_t port, uint16_t value) __attribute__(( nothrow, always_inline ));
inline void outw(uint16_t port, uint16_t value)
{
	asm volatile (
		"mov %0, %%ax;\n"
		"mov %1, %%dx;\n"
		"outw %%ax, %%dx;\n"
		:
		: "m" (value), "m" (port)
		: "ax", "dx"
	);
}

inline void outl(uint16_t port, uint32_t value) __attribute__(( nothrow, always_inline ));
inline void outl(uint16_t port, uint32_t value)
{
	asm volatile (
		"mov %0, %%eax;\n"
		"mov %1, %%dx;\n"
		"outl %%eax, %%dx;\n"
		:
		: "m" (value), "m" (port)
		: "eax", "dx"
	);
}

