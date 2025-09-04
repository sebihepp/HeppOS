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
physical_address_t CPMM::mMemoryLowStack = (physical_address_t)NULL;
physical_address_t CPMM::mMemoryHighStack = (physical_address_t)NULL;

uint64_t CPMM::mFreeMemoryAmount = 0;
uint64_t CPMM::mUsedMemoryAmount = 0;
	
#define ACCESS_PHYS_ADDR(address, type) (*((type*)((uintptr_t)address + (uintptr_t)CPaging::GetHHDMOffset())))

ReturnValue_t CPMM::PreInit(void) {

	// Set entire bitmap to "used"
	for (uint64_t i = 0; i < PMM_ISA_BITMAP_SIZE; ++i) {
		mMemoryISABitmap[i] = ~0;
	}
	
	limine_memmap_response *_LimineMemoryMapResponse = CLimine::GetMemoryMapResponse();

	// Set ISA Memory (below 1MB)
	for (size_t i = 0; i < _LimineMemoryMapResponse->entry_count ; ++i) {
		limine_memmap_entry *_LimineMemoryMapEntry = _LimineMemoryMapResponse->entries[i];
		
		// Skip not usable memory
		if (_LimineMemoryMapEntry->type != LIMINE_MEMMAP_USABLE)
			continue;
		
		
	}
	
	physical_address_t _MemoryTest = (physical_address_t)NULL;
	
	ReturnValue_t _RetVal = AllocISA(_MemoryTest, 1);
	if (IS_ERROR(_RetVal)) {
		return _RetVal;
	}
	FreeISA(_MemoryTest, 1);
	
	return RETVAL_OK;
}

ReturnValue_t CPMM::Init(void) {
	
	return RETVAL_ERROR_GENERAL;
}

ReturnValue_t CPMM::Alloc(physical_address_t &pAddress) {
	ReturnValue_t _RetVal = RETVAL_ERROR_GENERAL;
	
	//Try allocating High Memory
	_RetVal = AllocHigh(pAddress);
	if (IS_SUCCESS(_RetVal))
		return _RetVal;
	
	//Try allocating Low Memory next
	_RetVal = AllocLow(pAddress);
	if (IS_SUCCESS(_RetVal))
		return _RetVal;
	
	//Try ISA memory as a last resort
	_RetVal = AllocISA(pAddress, 1);
	
	return _RetVal;
}

ReturnValue_t CPMM::AllocISA(physical_address_t &pAddress, size_t pPageCount) {
	
	//Dont forget to adjust mFreeMemoryAmount and mUsedMemoryAmount
	
	pAddress = (physical_address_t)NULL;
	return RETVAL_ERROR_OOM_PHYSICAL;
}

ReturnValue_t CPMM::AllocLow(physical_address_t &pAddress) {
	
	if (mMemoryLowStack == (physical_address_t)NULL) {
		pAddress = (physical_address_t)NULL;
		return RETVAL_ERROR_OOM_PHYSICAL;
	}
	
	pAddress = (physical_address_t)mMemoryLowStack;
	mMemoryLowStack = ACCESS_PHYS_ADDR(mMemoryLowStack, physical_address_t);
	mFreeMemoryAmount -= PAGE_SIZE;
	mUsedMemoryAmount += PAGE_SIZE;
	
	return RETVAL_OK;
}

ReturnValue_t CPMM::AllocHigh(physical_address_t &pAddress) {
	
	if (mMemoryHighStack == (physical_address_t)NULL) {
		pAddress = (physical_address_t)NULL;
		return RETVAL_ERROR_OOM_PHYSICAL;
	}
	
	pAddress = mMemoryHighStack;
	mMemoryHighStack = ACCESS_PHYS_ADDR(mMemoryHighStack, physical_address_t);
	mFreeMemoryAmount -= PAGE_SIZE;
	mUsedMemoryAmount += PAGE_SIZE;
	
	return RETVAL_OK;
}

void CPMM::Free(physical_address_t pAddress) {
	
	pAddress = ((physical_address_t)pAddress) & ~0xFFF;
	
	if (pAddress == (physical_address_t)NULL)
		return;
	
	if (pAddress >= MEMORY_LOW_END) {
		FreeHigh(pAddress);
	} else if (pAddress >= MEMORY_ISA_END) {
		FreeLow(pAddress);
	} else {
		FreeISA(pAddress, 1);
	}
}

void CPMM::FreeISA(physical_address_t pAddress, size_t pPageCount) {
	
	pAddress = ((physical_address_t)pAddress) & ~0xFFF;
	
	
	mFreeMemoryAmount += PAGE_SIZE * pPageCount;
	mUsedMemoryAmount -= PAGE_SIZE + pPageCount;
}

void CPMM::FreeLow(physical_address_t pAddress) {
	if (mMemoryLowStack == (physical_address_t)NULL) {
		mMemoryLowStack = pAddress;
		ACCESS_PHYS_ADDR(pAddress, physical_address_t) = (physical_address_t)NULL;
		return;
	}
	
	ACCESS_PHYS_ADDR(pAddress, physical_address_t) = mMemoryLowStack;
	mMemoryLowStack = pAddress;
	
	mFreeMemoryAmount += PAGE_SIZE;
	mUsedMemoryAmount -= PAGE_SIZE;
}

void CPMM::FreeHigh(physical_address_t pAddress) {
	if (mMemoryHighStack == (physical_address_t)NULL) {
		mMemoryHighStack = pAddress;
		ACCESS_PHYS_ADDR(pAddress, physical_address_t) = (physical_address_t)NULL;
		return;
	}
	
	ACCESS_PHYS_ADDR(pAddress, physical_address_t) = mMemoryHighStack;
	mMemoryHighStack = pAddress;
	mFreeMemoryAmount += PAGE_SIZE;
	mUsedMemoryAmount -= PAGE_SIZE;
}

uint64_t CPMM::GetFreeMemory(void) {
	return mFreeMemoryAmount;
}

uint64_t CPMM::GetUsedMemory(void) {
	return mUsedMemoryAmount;
}

