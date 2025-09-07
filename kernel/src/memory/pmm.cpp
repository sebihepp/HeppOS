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
PhysicalAddress_t CPMM::mMemoryLowStack = (PhysicalAddress_t)NULL;
PhysicalAddress_t CPMM::mMemoryHighStack = (PhysicalAddress_t)NULL;

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
		
		for (PhysicalAddress_t _CurrentAddress = _LimineMemoryMapEntry->base;
				_CurrentAddress < (PhysicalAddress_t)(_LimineMemoryMapEntry->base + _LimineMemoryMapEntry->length);
				_CurrentAddress += PAGE_SIZE) 
		{
			CPMM::Free(_CurrentAddress);		
		}
	}
	
	// ToDo: 
	// Add the Amount of non usable memory to mUsedMemoryAmount
	mUsedMemoryAmount += 0;
	
	PhysicalAddress_t _MemoryTest = (PhysicalAddress_t)NULL;
	
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

ReturnValue_t CPMM::Alloc(PhysicalAddress_t &pAddress) {
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

ReturnValue_t CPMM::AllocISA(PhysicalAddress_t &pAddress, size_t pPageCount) {
	
	
	// ToDo
	// Implement searching, marking used and returning pPageCount continous ISA memory
	// Dont forget to adjust mFreeMemoryAmount and mUsedMemoryAmount
	
	
	
	pAddress = (PhysicalAddress_t)NULL;
	return RETVAL_ERROR_OOM_PHYSICAL;
}

ReturnValue_t CPMM::AllocLow(PhysicalAddress_t &pAddress) {
	
	if (mMemoryLowStack == (PhysicalAddress_t)NULL) {
		pAddress = (PhysicalAddress_t)NULL;
		return RETVAL_ERROR_OOM_PHYSICAL;
	}
	
	pAddress = (PhysicalAddress_t)mMemoryLowStack;
	mMemoryLowStack = ACCESS_PHYS_ADDR(mMemoryLowStack, PhysicalAddress_t);
	if (mFreeMemoryAmount != 0)
		mFreeMemoryAmount -= PAGE_SIZE;
	mUsedMemoryAmount += PAGE_SIZE;
	
	return RETVAL_OK;
}

ReturnValue_t CPMM::AllocHigh(PhysicalAddress_t &pAddress) {
	
	if (mMemoryHighStack == (PhysicalAddress_t)NULL) {
		pAddress = (PhysicalAddress_t)NULL;
		return RETVAL_ERROR_OOM_PHYSICAL;
	}
	
	pAddress = mMemoryHighStack;
	mMemoryHighStack = ACCESS_PHYS_ADDR(mMemoryHighStack, PhysicalAddress_t);
	if (mFreeMemoryAmount != 0)
		mFreeMemoryAmount -= PAGE_SIZE;
	mUsedMemoryAmount += PAGE_SIZE;
	
	return RETVAL_OK;
}

void CPMM::Free(PhysicalAddress_t pAddress) {
	
	pAddress = ((PhysicalAddress_t)pAddress) & ~0xFFF;
	
	if (pAddress == (PhysicalAddress_t)NULL)
		return;
	
	if (pAddress >= MEMORY_LOW_END) {
		FreeHigh(pAddress);
	} else if (pAddress >= MEMORY_ISA_END) {
		FreeLow(pAddress);
	} else {
		FreeISA(pAddress, 1);
	}
}

void CPMM::FreeISA(PhysicalAddress_t pAddress, size_t pPageCount) {
	
	pAddress = ((PhysicalAddress_t)pAddress) & ~0xFFF;
	
	// ToDo
	// Implement freeing the page in mMemoryISABitmap
	
	
	mFreeMemoryAmount += PAGE_SIZE * pPageCount;
	if (mUsedMemoryAmount != 0)
		mUsedMemoryAmount -= PAGE_SIZE + pPageCount;
}

void CPMM::FreeLow(PhysicalAddress_t pAddress) {
	if (mMemoryLowStack == (PhysicalAddress_t)NULL) {
		mMemoryLowStack = pAddress;
		ACCESS_PHYS_ADDR(pAddress, PhysicalAddress_t) = (PhysicalAddress_t)NULL;
		return;
	}
	
	ACCESS_PHYS_ADDR(pAddress, PhysicalAddress_t) = mMemoryLowStack;
	mMemoryLowStack = pAddress;
	
	mFreeMemoryAmount += PAGE_SIZE;
	if (mUsedMemoryAmount != 0)
		mUsedMemoryAmount -= PAGE_SIZE;
}

void CPMM::FreeHigh(PhysicalAddress_t pAddress) {
	if (mMemoryHighStack == (PhysicalAddress_t)NULL) {
		mMemoryHighStack = pAddress;
		ACCESS_PHYS_ADDR(pAddress, PhysicalAddress_t) = (PhysicalAddress_t)NULL;
		return;
	}
	
	ACCESS_PHYS_ADDR(pAddress, PhysicalAddress_t) = mMemoryHighStack;
	mMemoryHighStack = pAddress;
	mFreeMemoryAmount += PAGE_SIZE;
	if (mUsedMemoryAmount != 0)
		mUsedMemoryAmount -= PAGE_SIZE;
}
