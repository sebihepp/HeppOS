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

inline uint8_t mmio_inb(void *address) 
    __attribute__(( nothrow, always_inline ));
inline uint8_t mmio_inb(void *address)
{
	return *reinterpret_cast<volatile uint8_t*>(address);
}

inline uint16_t mmio_inw(void *address) 
    __attribute__(( nothrow, always_inline ));
inline uint16_t mmio_inw(void *address)
{
	return *reinterpret_cast<volatile uint16_t*>(address);
}

inline uint32_t mmio_inl(void *address) 
    __attribute__(( nothrow, always_inline ));
inline uint32_t mmio_inl(void *address)
{
	return *reinterpret_cast<volatile uint32_t*>(address);
}


inline void mmio_outb(void *address, uint8_t value) 
    __attribute__(( nothrow, always_inline ));
inline void mmio_outb(void *address, uint8_t value)
{
	*reinterpret_cast<volatile uint8_t*>(address) = value;
}

inline void mmio_outw(void *address, uint16_t value) 
    __attribute__(( nothrow, always_inline ));
inline void mmio_outw(void *address, uint16_t value)
{
	*reinterpret_cast<volatile uint16_t*>(address) = value;
}

inline void mmio_outl(void *address, uint32_t value) 
    __attribute__(( nothrow, always_inline ));
inline void mmio_outl(void *address, uint32_t value)
{
	*reinterpret_cast<volatile uint32_t*>(address) = value;
}

