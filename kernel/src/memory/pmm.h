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

#include <retval.h>
#include <memory/paging.h>

#define MEMORY_ISA_END (1024*1024*16)
#define MEMORY_LOW_END (1024*1024*1024*4)

struct MemoryRange_t {
	
	size_t Size;
	MemoryRange_t *ListNext;
	MemoryRange_t *ListPrev;
};

class CPMM {
private:
	
	static MemoryRange_t *mMemoryISAList;	//Memory below 16MB
	static MemoryRange_t *mMemoryLowList;	//Memory between 16MB and 4GB
	static MemoryRange_t *mMemoryHighList;	//Memory above 4GB
	
	CPMM() = delete;
	~CPMM() = delete;
public:

	static ReturnValue_t PreInit(void) __attribute__ (( nothrow ));
	static ReturnValue_t Init(void) __attribute__ (( nothrow ));
	
	static void PrintMemoryMap(void) __attribute__ (( nothrow ));
	
};
