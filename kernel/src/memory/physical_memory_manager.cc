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

#include <memory/physical_memory_manager.h>
#include <limine_stub.h>
#include <memory/paging.h>
#include <boot_log.h>

uint32_t PhysicalMemoryManager::mMemoryISABitmap[PMM_ISA_BITMAP_SIZE];
PhysicalAddress PhysicalMemoryManager::mMemoryLowStack = (PhysicalAddress)NULL;
PhysicalAddress PhysicalMemoryManager::mMemoryHighStack = (PhysicalAddress)NULL;

uint64_t PhysicalMemoryManager::mFreeMemoryAmount = 0;
uint64_t PhysicalMemoryManager::mUsedMemoryAmount = 0;
	
#define ACCESS_PHYS_ADDR(address, type) (*((type*)((uintptr_t)address + (uintptr_t)Paging::GetHHDMOffset())))

ReturnValue PhysicalMemoryManager::PreInit(void) {

	// Set entire bitmap to "used"
	for (uint64_t i = 0; i < PMM_ISA_BITMAP_SIZE; ++i) {
		mMemoryISABitmap[i] = ~0;
	}
	
	limine_memmap_response *_LimineMemoryMapResponse = LimineStub::GetMemoryMapResponse();
	uint64_t _UsedMemoryAmount = 0;
	uint64_t _FreeMemoryAmount = 0;
	
	// Set ISA Memory (below 1MB)
	for (size_t i = 0; i < _LimineMemoryMapResponse->entry_count ; ++i) {
		limine_memmap_entry *_LimineMemoryMapEntry = _LimineMemoryMapResponse->entries[i];
		
		// Skip not usable memory
		if (_LimineMemoryMapEntry->type == LIMINE_MEMMAP_USABLE) {
			for (PhysicalAddress _CurrentAddress = _LimineMemoryMapEntry->base;
					_CurrentAddress < (PhysicalAddress)(_LimineMemoryMapEntry->base + _LimineMemoryMapEntry->length);
					_CurrentAddress += PAGE_SIZE) 
			{
				Free(_CurrentAddress);
				_FreeMemoryAmount += PAGE_SIZE;
			}
		} else if (_LimineMemoryMapEntry->type == LIMINE_MEMMAP_EXECUTABLE_AND_MODULES) {
			_UsedMemoryAmount += _LimineMemoryMapEntry->length;
		}
	}
	
	// ToDo: 
	// Add the Amount of non usable memory to mUsedMemoryAmount
	mUsedMemoryAmount = _UsedMemoryAmount;
	mFreeMemoryAmount = _FreeMemoryAmount;
	
	PhysicalAddress _MemoryTest = (PhysicalAddress)NULL;
	
	ReturnValue RetVal = AllocISA(_MemoryTest, 1);
	if (IS_ERROR(RetVal)) {
		return RetVal;
	}
	FreeISA(_MemoryTest, 1);
	
	return ReturnValueOk;
}

ReturnValue PhysicalMemoryManager::Init(void) {
	
	return ReturnValueErrorGeneral;
}

ReturnValue PhysicalMemoryManager::Alloc(PhysicalAddress &address) {
	ReturnValue RetVal = ReturnValueErrorGeneral;
	
	//Try allocating High Memory
	RetVal = AllocHigh(address);
	if (IS_SUCCESS(RetVal))
		return RetVal;
	
	//Try allocating Low Memory next
	RetVal = AllocLow(address);
	if (IS_SUCCESS(RetVal))
		return RetVal;
	
	//Try ISA memory as a last resort
	RetVal = AllocISA(address, 1);
	
	return RetVal;
}

ReturnValue PhysicalMemoryManager::AllocISA(PhysicalAddress &address, size_t page_count) {
	
	uint32_t _CurrentPage = 0;
	
	while (_CurrentPage < (PMM_ISA_BITMAP_SIZE * 32)) {
		
		uint32_t _Amount = ISACheck(_CurrentPage, true);
		if (_Amount == 0) {
			++_CurrentPage;
		} else if (_Amount < page_count) {
			_CurrentPage += _Amount + 1;
		} else {
			ISAMark(_CurrentPage, page_count, false);
			mFreeMemoryAmount -= PAGE_SIZE * page_count;
			mUsedMemoryAmount += PAGE_SIZE * page_count;
			address = (PhysicalAddress)(_CurrentPage * PAGE_SIZE);
			return ReturnValueOk;
		}
	}
	
	address = (PhysicalAddress)NULL;
	return ReturnValueErrorOutOfMemoryPhysical;
}

ReturnValue PhysicalMemoryManager::AllocLow(PhysicalAddress &address) {
	
	if (mMemoryLowStack == (PhysicalAddress)NULL) {
		address = (PhysicalAddress)NULL;
		return ReturnValueErrorOutOfMemoryPhysical;
	}
	
	address = (PhysicalAddress)mMemoryLowStack;
	mMemoryLowStack = ACCESS_PHYS_ADDR(mMemoryLowStack, PhysicalAddress);
	mFreeMemoryAmount -= PAGE_SIZE;
	mUsedMemoryAmount += PAGE_SIZE;
	
	return ReturnValueOk;
}

ReturnValue PhysicalMemoryManager::AllocHigh(PhysicalAddress &address) {
	
	if (mMemoryHighStack == (PhysicalAddress)NULL) {
		address = (PhysicalAddress)NULL;
		return ReturnValueErrorOutOfMemoryPhysical;
	}
	
	address = mMemoryHighStack;
	mMemoryHighStack = ACCESS_PHYS_ADDR(mMemoryHighStack, PhysicalAddress);
	if (mFreeMemoryAmount != 0)
		mFreeMemoryAmount -= PAGE_SIZE;
	mUsedMemoryAmount += PAGE_SIZE;
	
	return ReturnValueOk;
}

void PhysicalMemoryManager::Free(PhysicalAddress address) {
	
	address = ((PhysicalAddress)address) & ~0xFFF;
	
	if (address == (PhysicalAddress)NULL)
		return;
	
	if (address >= MEMORY_LOW_END) {
		FreeHigh(address);
	} else if (address >= MEMORY_ISA_END) {
		FreeLow(address);
	} else {
		FreeISA(address, 1);
	}
}

void PhysicalMemoryManager::FreeISA(PhysicalAddress address, size_t page_count) {
	
	address = ((PhysicalAddress)address) & ~0xFFF;
	
	ISAMark(address / PAGE_SIZE, page_count, true);
	
	mFreeMemoryAmount += PAGE_SIZE * page_count;
	mUsedMemoryAmount -= PAGE_SIZE * page_count;
}

void PhysicalMemoryManager::FreeLow(PhysicalAddress address) {
	if (mMemoryLowStack == (PhysicalAddress)NULL) {
		mMemoryLowStack = address;
		ACCESS_PHYS_ADDR(address, PhysicalAddress) = (PhysicalAddress)NULL;
		return;
	}
	
	ACCESS_PHYS_ADDR(address, PhysicalAddress) = mMemoryLowStack;
	mMemoryLowStack = address;
	
	mFreeMemoryAmount += PAGE_SIZE;
	if (mUsedMemoryAmount != 0)
		mUsedMemoryAmount -= PAGE_SIZE;
}

void PhysicalMemoryManager::FreeHigh(PhysicalAddress address) {
	if (mMemoryHighStack == (PhysicalAddress)NULL) {
		mMemoryHighStack = address;
		ACCESS_PHYS_ADDR(address, PhysicalAddress) = (PhysicalAddress)NULL;
		return;
	}
	
	ACCESS_PHYS_ADDR(address, PhysicalAddress) = mMemoryHighStack;
	mMemoryHighStack = address;
	mFreeMemoryAmount += PAGE_SIZE;
	if (mUsedMemoryAmount != 0)
		mUsedMemoryAmount -= PAGE_SIZE;
}

uint32_t PhysicalMemoryManager::ISACheck(uint32_t start, bool free) {
	
	uint32_t _Amount = 0;
	
	while (true) {
		uint32_t _Index = (start + _Amount) / 32;
		uint32_t _Bit = (start + _Amount) % 32;
	
		if (_Index >= PMM_ISA_BITMAP_SIZE) {
			return _Amount;
		}
		
		if (not free == (mMemoryISABitmap[_Index] & (1 << _Bit))) {
			++_Amount;
		} else {
			return _Amount;
		}
	}
	
	return _Amount;
}

void PhysicalMemoryManager::ISAMark(uint32_t start, size_t length, bool free) {
	
	for (uint32_t i = start; i < (start + length); ++i) {
		uint32_t _Index = i / 32;
		uint32_t _Bit = i % 32;
		
		if (_Index >= PMM_ISA_BITMAP_SIZE)
			break;
		
		if (free) {
			mMemoryISABitmap[_Index] &= ~(1 << _Bit);
		} else {
			mMemoryISABitmap[_Index] |= (1 << _Bit);
			
		}
	}	
	
}

