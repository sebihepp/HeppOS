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

#include <retval.h>
#include <memory/paging.h>

#define MEMORY_ISA_END ((uint64_t)((uint64_t)1024*1024*16))
#define MEMORY_LOW_END ((uint64_t)((uint64_t)1024*1024*1024*4))
#define PMM_ISA_BITMAP_SIZE (MEMORY_ISA_END / 4096 / 32)

class CPMM {
private:
	
	static uint32_t mMemoryISABitmap[PMM_ISA_BITMAP_SIZE];	//Memory below 16MB
	static PhysicalAddress_t mMemoryLowStack;				//Memory between 16MB and 4GB
	static PhysicalAddress_t mMemoryHighStack;				//Memory above 4GB
	
	static uint64_t mFreeMemoryAmount;
	static uint64_t mUsedMemoryAmount;
	
	static void FreeLow(PhysicalAddress_t pAddress) __attribute__ (( nothrow ));
	static void FreeHigh(PhysicalAddress_t pAddress) __attribute__ (( nothrow ));

	
	CPMM() = delete;
	~CPMM() = delete;
public:

	static ReturnValue_t PreInit(void) __attribute__ (( nothrow ));
	static ReturnValue_t Init(void) __attribute__ (( nothrow ));
	
	static ReturnValue_t Alloc(PhysicalAddress_t &pAddress) __attribute__ (( nothrow ));
	static ReturnValue_t AllocISA(PhysicalAddress_t &pAddress, size_t pPageCount) __attribute__ (( nothrow ));
	static ReturnValue_t AllocLow(PhysicalAddress_t &pAddress) __attribute__ (( nothrow ));
	static ReturnValue_t AllocHigh(PhysicalAddress_t &pAddress) __attribute__ (( nothrow ));
	
	static void Free(PhysicalAddress_t pAddress) __attribute__ (( nothrow ));
	static void FreeISA(PhysicalAddress_t pAddress, size_t pPageCount) __attribute__ (( nothrow ));

	static uint64_t GetFreeMemory(void) __attribute__ (( nothrow ));
	static uint64_t GetUsedMemory(void) __attribute__ (( nothrow ));
};
