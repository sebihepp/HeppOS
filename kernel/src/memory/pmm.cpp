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

#include <memory/pmm.h>
#include <liminestub.h>
#include <memory/paging.h>
#include <log.h>

uint32_t CPMM::mMemoryISABitmap[PMM_ISA_BITMAP_SIZE];
void *CPMM::mMemoryLowStack = NULL;
void *CPMM::mMemoryHighStack = NULL;

#define ACCESS_PHYS_ADDR(address, type) (*((type*)((uintptr_t)address + (uintptr_t)CPaging::GetHHDMOffset())))

ReturnValue_t CPMM::PreInit(void) {

	limine_memmap_response *_LimineMemoryMapResponse = CLimine::GetMemoryMapResponse();

	// Set ISA Memory (below 1MB)
	for (size_t i = 0; i < _LimineMemoryMapResponse->entry_count ; ++i) {
		limine_memmap_entry *_LimineMemoryMapEntry = _LimineMemoryMapResponse->entries[i];
		
		// Skip not usable memory
		if (_LimineMemoryMapEntry->type != LIMINE_MEMMAP_USABLE)
			continue;
		
		
	}
	
	return RETVAL_OK;
}

ReturnValue_t CPMM::Init(void) {
	
	return RETVAL_ERROR_GENERAL;
}

ReturnValue_t CPMM::Alloc(void **pAddress) {
	ReturnValue_t _RetVal = RETVAL_ERROR_GENERAL;
	
	//Try allocating High Memory
	_RetVal = AllocHigh(pAddress);
	if (IS_SUCCESS(_RetVal))
		return _RetVal;
	
	//Try allocating Low Memory next
	_RetVal = AllocLow(pAddress);
	return _RetVal;
}

ReturnValue_t CPMM::AllocISA(void **pAddress, size_t pSize) {
	
	pAddress = NULL;
	return RETVAL_ERROR_OOM_PHYSICAL;
}

ReturnValue_t CPMM::AllocLow(void **pAddress) {
	
	if (mMemoryLowStack == NULL) {
		pAddress = NULL;
		return RETVAL_ERROR_OOM_PHYSICAL;
	}
	
	*pAddress = mMemoryLowStack;
	mMemoryLowStack = ACCESS_PHYS_ADDR(mMemoryLowStack, void*);
	
	return RETVAL_OK;
}

ReturnValue_t CPMM::AllocHigh(void **pAddress) {
	
	if (mMemoryHighStack == NULL) {
		pAddress = NULL;
		return RETVAL_ERROR_OOM_PHYSICAL;
	}
	
	*pAddress = mMemoryHighStack;
	mMemoryHighStack = ACCESS_PHYS_ADDR(mMemoryHighStack, void*);
	
	return RETVAL_OK;
}

void CPMM::Free(void *pAddress) {
	if (((uintptr_t)pAddress) >= MEMORY_LOW_END) {
		FreeHigh(pAddress);
	} else {
		FreeLow(pAddress);
	}
}

void CPMM::FreeISA(void *pAddress, size_t pPageCount) {
	
}

void CPMM::FreeLow(void *pAddress) {
	if (mMemoryLowStack == NULL) {
		mMemoryLowStack = pAddress;
		ACCESS_PHYS_ADDR(pAddress, void*) = NULL;
		return;
	}
	
	ACCESS_PHYS_ADDR(pAddress, void*) = mMemoryLowStack;
	mMemoryLowStack = pAddress;
	
}

void CPMM::FreeHigh(void *pAddress) {
	if (mMemoryHighStack == NULL) {
		mMemoryHighStack = pAddress;
		ACCESS_PHYS_ADDR(pAddress, void*) = NULL;
		return;
	}
	
	ACCESS_PHYS_ADDR(pAddress, void*) = mMemoryHighStack;
	mMemoryHighStack = pAddress;
}
