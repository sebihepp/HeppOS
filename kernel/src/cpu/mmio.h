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

#ifndef HEADER_MMIO
#define HEADER_MMIO

#include <stddef.h>
#include <stdint.h>

inline uint8_t mmio_inb(void *pAddress) __attribute__(( nothrow, always_inline ));
inline uint8_t mmio_inb(void *pAddress)
{
	return *reinterpret_cast<volatile uint8_t*>(pAddress);
}

inline uint16_t mmio_inw(void *pAddress) __attribute__(( nothrow, always_inline ));
inline uint16_t mmio_inw(void *pAddress)
{
	return *reinterpret_cast<volatile uint16_t*>(pAddress);
}

inline uint32_t mmio_inl(void *pAddress) __attribute__(( nothrow, always_inline ));
inline uint32_t mmio_inl(void *pAddress)
{
	return *reinterpret_cast<volatile uint32_t*>(pAddress);
}


inline void mmio_outb(void *pAddress, uint8_t pValue) __attribute__(( nothrow, always_inline ));
inline void mmio_outb(void *pAddress, uint8_t pValue)
{
	*reinterpret_cast<volatile uint8_t*>(pAddress) = pValue;
}

inline void mmio_outw(void *pAddress, uint16_t pValue) __attribute__(( nothrow, always_inline ));
inline void mmio_outw(void *pAddress, uint16_t pValue)
{
	*reinterpret_cast<volatile uint16_t*>(pAddress) = pValue;
}

inline void mmio_outl(void *pAddress, uint32_t pValue) __attribute__(( nothrow, always_inline ));
inline void mmio_outl(void *pAddress, uint32_t pValue)
{
	*reinterpret_cast<volatile uint32_t*>(pAddress) = pValue;
}

#endif

