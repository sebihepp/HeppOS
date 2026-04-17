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

#define MEMORY_ISA_END ((PhysicalAddress_t)((uint64_t)1024*1024*16))
#define MEMORY_LOW_END ((PhysicalAddress_t)((uint64_t)1024*1024*1024*4))

struct MemoryRange_t {
	
	size_t Size;
	MemoryRange_t *ListNext;
	MemoryRange_t *ListPrev;
	MemoryRange_t *TreeAddressLeft;
	MemoryRange_t *TreeAddressRight;
	MemoryRange_t *TreeSizeLeft;
	MemoryRange_t *TreeSizeRight;
} __attribute__ (( aligned (4096) , packed ));

class CPMM {
private:
	
	static MemoryRange_t *mMemoryISAList;	//Memory below 16MB
	static MemoryRange_t *mMemoryLowList;	//Memory between 16MB and 4GB
	static MemoryRange_t *mMemoryHighList;	//Memory above 4GB
	
	static uint64_t mFreeMemoryAmount;
	static uint64_t mUsedMemoryAmount;

	static void SetISAFree(PhysicalAddress_t pBase, size_t pSize) __attribute__ (( nothrow ));
	static void SetISAUsed(PhysicalAddress_t pBase, size_t pSize) __attribute__ (( nothrow ));
	static void SetLowFree(PhysicalAddress_t pBase, size_t pSize) __attribute__ (( nothrow ));
	static void SetLowUsed(PhysicalAddress_t pBase, size_t pSize) __attribute__ (( nothrow ));
	static void SetHighFree(PhysicalAddress_t pBase, size_t pSize) __attribute__ (( nothrow ));
	static void SetHighUsed(PhysicalAddress_t pBase, size_t pSize) __attribute__ (( nothrow ));
	
	
	CPMM() = delete;
	~CPMM() = delete;
public:

	static ReturnValue_t PreInit(void) __attribute__ (( nothrow ));
	static ReturnValue_t Init(void) __attribute__ (( nothrow ));
	
	static void PrintMemoryMap(void) __attribute__ (( nothrow ));
	
	static void MergeISA(void) __attribute__ (( nothrow ));
	static void MergeLow(void) __attribute__ (( nothrow ));
	static void MergeHigh(void) __attribute__ (( nothrow ));

	static void SetFree(PhysicalAddress_t pBase, size_t pSize) __attribute__ (( nothrow ));
	static void SetUsed(PhysicalAddress_t pBase, size_t pSize) __attribute__ (( nothrow ));

	static ReturnValue_t Alloc(PhysicalAddress_t &pAddress, size_t pSize) __attribute__ (( nothrow ));
	static ReturnValue_t AllocISA(PhysicalAddress_t &pAddress, size_t pSize) __attribute__ (( nothrow ));
	static ReturnValue_t AllocLow(PhysicalAddress_t &pAddress, size_t pSize) __attribute__ (( nothrow ));
	static ReturnValue_t AllocHigh(PhysicalAddress_t &pAddress, size_t pSize) __attribute__ (( nothrow ));
	
	static void Free(PhysicalAddress_t pAddress, size_t pSize) __attribute__ (( nothrow ));
	
	static inline uint64_t GetFreeMemory(void) __attribute__ (( nothrow , always_inline )) {
		return mFreeMemoryAmount;
	}
	
	static inline uint64_t GetUsedMemory(void) __attribute__ (( nothrow , always_inline )) {
		return mUsedMemoryAmount;
	}

};
