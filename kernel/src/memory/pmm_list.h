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

#include <return_value.h>
#include <memory/paging.h>

#define MEMORY_ISA_END ((uint64_t)((uint64_t)1024*1024*16))
#define MEMORY_LOW_END ((uint64_t)((uint64_t)1024*1024*1024*4))

struct MemoryRange_t {
	
	size_t Size;
	MemoryRange_t *ListNext;
	MemoryRange_t *ListPrev;
} __attribute__((aligned(4096), packed));

class CPMM {
private:
	
	static MemoryRange_t *mMemoryISAList;	//Memory below 16MB
	static MemoryRange_t *mMemoryLowList;	//Memory between 16MB and 4GB
	static MemoryRange_t *mMemoryHighList;	//Memory above 4GB
	
	static void SetISAFree(void *base, size_t size) __attribute__((nothrow));
	static void SetISAUsed(void *base, size_t size) __attribute__((nothrow));
	static void SetLowFree(void *base, size_t size) __attribute__((nothrow));
	static void SetLowUsed(void *base, size_t size) __attribute__((nothrow));
	static void SetHighFree(void *base, size_t size) __attribute__((nothrow));
	static void SetHighUsed(void *base, size_t size) __attribute__((nothrow));
	
	static void MergeISA(void) __attribute__((nothrow));
	static void MergeLow(void) __attribute__((nothrow));
	static void MergeHigh(void) __attribute__((nothrow));
	
	
	CPMM() = delete;
	~CPMM() = delete;
public:

	static ReturnValue PreInit(void) __attribute__((nothrow));
	static ReturnValue Init(void) __attribute__((nothrow));
	
	static void PrintMemoryMap(void) __attribute__((nothrow));
	
	static void SetFree(void *base, size_t size) __attribute__((nothrow));
	static void SetUsed(void *base, size_t size) __attribute__((nothrow));

	static ReturnValue Alloc(void **address, size_t size) __attribute__((nothrow));
	static ReturnValue AllocISA(void **address, size_t size) __attribute__((nothrow));
	static ReturnValue AllocLow(void **address, size_t size) __attribute__((nothrow));
	static ReturnValue AllocHigh(void **address, size_t size) __attribute__((nothrow));
	
	static void Free(void *address, size_t size) __attribute__((nothrow));
	
};