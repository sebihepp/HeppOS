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

#include <memory/paging.h>

#define MEMORY_ISA_END ((PhysicalAddress)((uint64_t)1024*1024*16))
#define MEMORY_LOW_END ((PhysicalAddress)((uint64_t)1024*1024*1024*4))
#define PMM_ISA_BITMAP_SIZE (MEMORY_ISA_END / 4096 / 32)

class PhysicalMemoryManager {
private:
	
	static uint32_t mMemoryISABitmap[PMM_ISA_BITMAP_SIZE];	//Memory below 16MB - 1 means occupied - 0 means free
	static PhysicalAddress mMemoryLowStack;				//Memory between 16MB and 4GB
	static PhysicalAddress mMemoryHighStack;				//Memory above 4GB
	
	static uint64_t mFreeMemoryAmount;
	static uint64_t mUsedMemoryAmount;
	
	static void FreeLow(PhysicalAddress address) __attribute__((nothrow));
	static void FreeHigh(PhysicalAddress address) __attribute__((nothrow));

	static uint32_t ISACheck(uint32_t start, bool free_mem) __attribute__((nothrow));
	static void ISAMark(uint32_t start, size_t length, bool free_mem) __attribute__((nothrow));
	
	
	PhysicalMemoryManager() = delete;
	~PhysicalMemoryManager() = delete;
public:

	static ReturnValue PreInit(void) __attribute__((nothrow));
	static ReturnValue Init(void) __attribute__((nothrow));
	
	static ReturnValue Alloc(PhysicalAddress &address) __attribute__((nothrow));
	static ReturnValue AllocISA(PhysicalAddress &address,
	                              size_t page_count) __attribute__((nothrow));
	static ReturnValue AllocLow(PhysicalAddress &address) __attribute__((nothrow));
	static ReturnValue AllocHigh(PhysicalAddress &address) __attribute__((nothrow));
	
	static void Free(PhysicalAddress address) __attribute__((nothrow));
	static void FreeISA(PhysicalAddress address,
	                    size_t page_count) __attribute__((nothrow));

	static inline uint64_t GetFreeMemory(void)
	    __attribute__((nothrow, always_inline)) {
		return mFreeMemoryAmount;
	}
	
	static inline uint64_t GetUsedMemory(void)
	    __attribute__((nothrow, always_inline)) {
		return mUsedMemoryAmount;
	}
	
};
