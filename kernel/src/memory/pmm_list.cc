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

#include <memory/pmm_list.h>
#include <limine_stub.h>
#include <memory/paging.h>
#include <boot_log.h>

MemoryRange_t *CPMM::mMemoryISAList = NULL;
MemoryRange_t *CPMM::mMemoryLowList = NULL;
MemoryRange_t *CPMM::mMemoryHighList = NULL;


ReturnValue CPMM::PreInit(void) {

	limine_memmap_response *_LimineMemoryMapResponse = LimineStub::GetMemoryMapResponse();

	// Set ISA Memory (below 1MB)
	for (size_t i = 0; i < _LimineMemoryMapResponse->entry_count ; ++i) {
		limine_memmap_entry *_LimineMemoryMapEntry = _LimineMemoryMapResponse->entries[i];
		
		// Skip not usable memory
		if (_LimineMemoryMapEntry->type != LIMINE_MEMMAP_USABLE)
			continue;
		
		
		SetISAFree((void*)_LimineMemoryMapEntry->base, _LimineMemoryMapEntry->length);	
		SetLowFree((void*)_LimineMemoryMapEntry->base, _LimineMemoryMapEntry->length);
		SetHighFree((void*)_LimineMemoryMapEntry->base, _LimineMemoryMapEntry->length);
		
	}
	
	if (mMemoryISAList == NULL)
		return RetValErrorGeneral;
	
	return RetValOk;
}

ReturnValue CPMM::Init(void) {
	
	return RetValErrorGeneral;
}


void CPMM::PrintMemoryMap(void) {
	
	BootLog::Print("MemoryMap:\n");
	BootLog::Print("Start              | End                | Size              \n");
	BootLog::Print("------------------------------------------------------------\n");
	
	MemoryRange_t *_CurrentRange = NULL;
	
	if (mMemoryISAList != NULL) {
		_CurrentRange = mMemoryISAList;
		while (_CurrentRange != NULL) {
			
			BootLog::PrintF("%016p | %016p | %016p\n", (uintptr_t)_CurrentRange - (uintptr_t)Paging::GetHHDMOffset(), 
				(uintptr_t)_CurrentRange - (uintptr_t)Paging::GetHHDMOffset() + _CurrentRange->Size - 1,
				_CurrentRange->Size);
			_CurrentRange = _CurrentRange->ListNext;
			
		}
	}

	if (mMemoryLowList != NULL) {
		_CurrentRange = mMemoryLowList;
		while (_CurrentRange != NULL) {
			
			BootLog::PrintF("%016p | %016p | %016p\n", (uintptr_t)_CurrentRange - (uintptr_t)Paging::GetHHDMOffset(), 
				(uintptr_t)_CurrentRange - (uintptr_t)Paging::GetHHDMOffset() + _CurrentRange->Size - 1,
				_CurrentRange->Size);
			_CurrentRange = _CurrentRange->ListNext;
			
		}
	}
	
	if (mMemoryHighList != NULL) {
		_CurrentRange = mMemoryHighList;
		while (_CurrentRange != NULL) {
			
			BootLog::PrintF("%016p | %016p | %016p\n", (uintptr_t)_CurrentRange - (uintptr_t)Paging::GetHHDMOffset(), 
				(uintptr_t)_CurrentRange - (uintptr_t)Paging::GetHHDMOffset() + _CurrentRange->Size - 1,
				_CurrentRange->Size);
			_CurrentRange = _CurrentRange->ListNext;
			
		}
	}
}

void CPMM::SetISAFree(void *base, size_t size) {

	//Align base to PAGE_SIZE
	if ((uintptr_t)base & (PAGE_SIZE - 1)) {
		base = (void*)((uintptr_t)base + PAGE_SIZE);
		base = (void*)((uintptr_t)base & ~(PAGE_SIZE - 1));
	}
	
	//Align size to PAGE_SIZE
	if (size & (PAGE_SIZE - 1)) {
		size += PAGE_SIZE;
		size &= ~(PAGE_SIZE - 1);
	}
	
	// Ignore if above 1MB
	if ((uintptr_t)base >= MEMORY_ISA_END)
		return;
	
	// Cut Range above 1MB
	if (((uintptr_t)base + size) > MEMORY_ISA_END) {
		size = MEMORY_ISA_END - (uintptr_t)base;
	}

	//Check if Entry already exists
	MemoryRange_t *_CurrentEntry = mMemoryISAList;
	while (_CurrentEntry != NULL) {
		
		//Case: Base is the same
		if (_CurrentEntry == (MemoryRange_t*)((uintptr_t)base + (uintptr_t)Paging::GetHHDMOffset())) {
			// If new size is greater, then just update size
			if (size > _CurrentEntry->Size) {
				_CurrentEntry->Size = size;
				return;
			}
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	// Create MemoryRange Entry
	MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)base + (uintptr_t)Paging::GetHHDMOffset());
	_NewMemoryRangeEntry->Size = size;
	_NewMemoryRangeEntry->ListNext = NULL;
	_NewMemoryRangeEntry->ListPrev = NULL;
	
	// Add Entry to the list
	if (mMemoryISAList == NULL) {
		mMemoryISAList = _NewMemoryRangeEntry;
		return;
	} else {
		//Search List for Position to insert
		MemoryRange_t *_CurrentEntry = mMemoryISAList;
		
		while (_CurrentEntry != NULL) {
			
			// if found, add in between
			if ((uintptr_t)_CurrentEntry > (uintptr_t)_NewMemoryRangeEntry) {
				
				if (_CurrentEntry->ListPrev == NULL) {
					mMemoryISAList = _NewMemoryRangeEntry;
				} else {
					_CurrentEntry->ListPrev->ListNext = _NewMemoryRangeEntry;
					_NewMemoryRangeEntry->ListPrev = _CurrentEntry->ListPrev;
				}
				_CurrentEntry->ListPrev = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListNext = _CurrentEntry;
				break;
				
			// if Last entry, add to back of list
			} else if (_CurrentEntry->ListNext == NULL)  {
				_CurrentEntry->ListNext = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListPrev = _CurrentEntry;				
				break;
			}
			
			_CurrentEntry = _CurrentEntry->ListNext;
		}
		
	}
	
	MergeISA();
	
}

void CPMM::SetISAUsed(void *base, size_t size) {

	//Align base to PAGE_SIZE
	if ((uintptr_t)base & (PAGE_SIZE - 1)) {
		base = (void*)((uintptr_t)base + PAGE_SIZE);
		base = (void*)((uintptr_t)base & ~(PAGE_SIZE - 1));
	}
	
	//Align size to PAGE_SIZE
	if (size & (PAGE_SIZE - 1)) {
		size += PAGE_SIZE;
		size &= ~(PAGE_SIZE - 1);
	}
	
	// if there is no list we can directly return
	if (mMemoryISAList == NULL)
		return;

	// Ignore if above 1MB
	if ((uintptr_t)base >= MEMORY_ISA_END)
		return;
	
	uintptr_t _VirtualBase = (uintptr_t)base + (uintptr_t)Paging::GetHHDMOffset();
	
	// Loop over each Range
	MemoryRange_t *_CurrentEntry = mMemoryISAList;
	while (_CurrentEntry != NULL) {
		
		// Case: entire entry can be deleted
		if ((_VirtualBase <= (uintptr_t)_CurrentEntry) && 
			((_VirtualBase + size) >= ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
			
			if (_CurrentEntry->ListPrev == NULL) {
				mMemoryISAList = _CurrentEntry->ListNext;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = NULL;
				}
			} else {
				_CurrentEntry->ListPrev = _CurrentEntry->ListNext;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = _CurrentEntry->ListPrev;
				}
			}
		
		// Case: start of entry can be deleted
		} else if ((_VirtualBase <= (uintptr_t)_CurrentEntry) && 
			((_VirtualBase + size) > ((uintptr_t)_CurrentEntry)) &&
			((_VirtualBase + size) < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
		
			// Create MemoryRange Entry
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)base + size + (uintptr_t)Paging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (uintptr_t)_CurrentEntry + _CurrentEntry->Size - (uintptr_t)_NewMemoryRangeEntry;
			_NewMemoryRangeEntry->ListNext = NULL;
			_NewMemoryRangeEntry->ListPrev = NULL;
			
			// Replace _CurrentEntry with _NewMemoryRangeEntry
			if (_CurrentEntry->ListPrev == NULL) {
				mMemoryISAList = _NewMemoryRangeEntry;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = _NewMemoryRangeEntry;
					_NewMemoryRangeEntry->ListNext = _CurrentEntry->ListNext;
				}
			} else {
				_CurrentEntry->ListPrev->ListNext = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListPrev = _CurrentEntry->ListPrev;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = _NewMemoryRangeEntry;
					_NewMemoryRangeEntry->ListNext = _CurrentEntry->ListNext;
				}
			}
		
		// Case: end of entry can be deleted
		} else if ((_VirtualBase > (uintptr_t)_CurrentEntry) && 
			((_VirtualBase) < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) &&
			((_VirtualBase + size) >= ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
	
			//Only adjust size
			_CurrentEntry->Size = _VirtualBase - (uintptr_t)_CurrentEntry;
			
			
		// Case: CurrentEntry needs to be split
		} else if ((_VirtualBase > (uintptr_t)_CurrentEntry) && 
			(_VirtualBase < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) && 
			((_VirtualBase + size) < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
			
			//Create new Entry for the range at the end
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)base + size + (uintptr_t)Paging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (uintptr_t)_CurrentEntry + _CurrentEntry->Size - (uintptr_t)_NewMemoryRangeEntry;
			_NewMemoryRangeEntry->ListNext = NULL;
			_NewMemoryRangeEntry->ListPrev = NULL;
			
			
			// Insert _NewMemoryRangeEntry behind current Entry
			if (_CurrentEntry->ListNext != NULL) {
				_CurrentEntry->ListNext->ListPrev = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListNext = _CurrentEntry->ListNext;
			}	
			_CurrentEntry->ListNext = _NewMemoryRangeEntry;
			_NewMemoryRangeEntry->ListPrev = _CurrentEntry;
			
			//adjust size
			_CurrentEntry->Size = _VirtualBase - (uintptr_t)_CurrentEntry;
		}

		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
}

void CPMM::SetLowFree(void *base, size_t size) {
	
	//Align base to PAGE_SIZE
	if ((uintptr_t)base & (PAGE_SIZE - 1)) {
		base = (void*)((uintptr_t)base + PAGE_SIZE);
		base = (void*)((uintptr_t)base & ~(PAGE_SIZE - 1));
	}
	
	//Align size to PAGE_SIZE
	if (size & (PAGE_SIZE - 1)) {
		size += PAGE_SIZE;
		size &= ~(PAGE_SIZE - 1);
	}
	
	// Ignore if above 4GB
	if ((uintptr_t)base >= MEMORY_LOW_END)
		return;

	// Ignore below 1MB
	if (((uintptr_t)base + (uintptr_t)size) < MEMORY_ISA_END)
		return;
	
	// Cut Range below 1MB
	if ((uintptr_t)base < MEMORY_ISA_END) {
		size -= MEMORY_ISA_END - (uintptr_t)base;
		base = (void*)MEMORY_ISA_END;
	}
	
	// Cut Range above 4GB
	if (((uintptr_t)base + size) > MEMORY_LOW_END) {
		size = MEMORY_LOW_END - (uintptr_t)base;
	}

	//Check if Entry already exists
	MemoryRange_t *_CurrentEntry = mMemoryLowList;
	while (_CurrentEntry != NULL) {
		
		//Case: Base is the same
		if (_CurrentEntry == (MemoryRange_t*)((uintptr_t)base + (uintptr_t)Paging::GetHHDMOffset())) {
			// If new size is greater, then just update size
			if (size > _CurrentEntry->Size) {
				_CurrentEntry->Size = size;
				return;
			}
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	// Create MemoryRange Entry
	MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)base + (uintptr_t)Paging::GetHHDMOffset());
	_NewMemoryRangeEntry->Size = size;
	_NewMemoryRangeEntry->ListNext = NULL;
	_NewMemoryRangeEntry->ListPrev = NULL;
	
	// Add Entry to the list
	if (mMemoryLowList == NULL) {
		mMemoryLowList = _NewMemoryRangeEntry;
		return;
	} else {
		//Search List for Position to insert
		MemoryRange_t *_CurrentEntry = mMemoryLowList;
		
		while (_CurrentEntry != NULL) {
			
			// if found, add in between
			if ((uintptr_t)_CurrentEntry > (uintptr_t)_NewMemoryRangeEntry) {
			
				if (_CurrentEntry->ListPrev == NULL) {
					mMemoryLowList = _NewMemoryRangeEntry;
				} else {
					_CurrentEntry->ListPrev->ListNext = _NewMemoryRangeEntry;
					_NewMemoryRangeEntry->ListPrev = _CurrentEntry->ListPrev;
				}
				_CurrentEntry->ListPrev = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListNext = _CurrentEntry;
				break;
			}
			
			// if Last entry, add to back of list
			if (_CurrentEntry->ListNext == NULL)  {
				_CurrentEntry->ListNext = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListPrev = _CurrentEntry;				
				break;
			}
			
			_CurrentEntry = _CurrentEntry->ListNext;
		}
		
	}
	
	MergeLow();
	
}

void CPMM::SetLowUsed(void *base, size_t size) {
	
	//Align base to PAGE_SIZE
	if ((uintptr_t)base & (PAGE_SIZE - 1)) {
		base = (void*)((uintptr_t)base + PAGE_SIZE);
		base = (void*)((uintptr_t)base & ~(PAGE_SIZE - 1));
	}
	
	//Align size to PAGE_SIZE
	if (size & (PAGE_SIZE - 1)) {
		size += PAGE_SIZE;
		size &= ~(PAGE_SIZE - 1);
	}
	
	// Ignore if above 4GB
	if ((uintptr_t)base >= MEMORY_LOW_END)
		return;

	// Ignore below 1MB
	if (((uintptr_t)base + (uintptr_t)size) < MEMORY_ISA_END)
		return;
	
	
	uintptr_t _VirtualBase = (uintptr_t)base + (uintptr_t)Paging::GetHHDMOffset();
	
	// Loop over each Range
	MemoryRange_t *_CurrentEntry = mMemoryLowList;
	while (_CurrentEntry != NULL) {
		
		// Case: entire entry can be deleted
		if ((_VirtualBase <= (uintptr_t)_CurrentEntry) && 
			((_VirtualBase + size) >= ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
			
			if (_CurrentEntry->ListPrev == NULL) {
				mMemoryLowList = _CurrentEntry->ListNext;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = NULL;
				}
			} else {
				_CurrentEntry->ListPrev = _CurrentEntry->ListNext;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = _CurrentEntry->ListPrev;
				}
			}
		
		// Case: start of entry can be deleted
		} else if ((_VirtualBase <= (uintptr_t)_CurrentEntry) && 
			((_VirtualBase + size) > ((uintptr_t)_CurrentEntry)) &&
			((_VirtualBase + size) < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
		
			// Create MemoryRange Entry
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)base + size + (uintptr_t)Paging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (uintptr_t)_CurrentEntry + _CurrentEntry->Size - (uintptr_t)_NewMemoryRangeEntry;
			_NewMemoryRangeEntry->ListNext = NULL;
			_NewMemoryRangeEntry->ListPrev = NULL;
			
			// Replace _CurrentEntry with _NewMemoryRangeEntry
			if (_CurrentEntry->ListPrev == NULL) {
				mMemoryLowList = _NewMemoryRangeEntry;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = _NewMemoryRangeEntry;
					_NewMemoryRangeEntry->ListNext = _CurrentEntry->ListNext;
				}
			} else {
				_CurrentEntry->ListPrev->ListNext = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListPrev = _CurrentEntry->ListPrev;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = _NewMemoryRangeEntry;
					_NewMemoryRangeEntry->ListNext = _CurrentEntry->ListNext;
				}
			}
		
		// Case: end of entry can be deleted
		} else if ((_VirtualBase > (uintptr_t)_CurrentEntry) && 
			((_VirtualBase) < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) &&
			((_VirtualBase + size) >= ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
	
			//Only adjust size
			_CurrentEntry->Size = _VirtualBase - (uintptr_t)_CurrentEntry;
			
			
		// Case: CurrentEntry needs to be split
		} else if ((_VirtualBase > (uintptr_t)_CurrentEntry) && 
			(_VirtualBase < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) && 
			((_VirtualBase + size) < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
			
			//Create new Entry for the range at the end
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)base + size + (uintptr_t)Paging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (uintptr_t)_CurrentEntry + _CurrentEntry->Size - (uintptr_t)_NewMemoryRangeEntry;
			_NewMemoryRangeEntry->ListNext = NULL;
			_NewMemoryRangeEntry->ListPrev = NULL;
			
			
			// Insert _NewMemoryRangeEntry behind current Entry
			if (_CurrentEntry->ListNext != NULL) {
				_CurrentEntry->ListNext->ListPrev = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListNext = _CurrentEntry->ListNext;
			}	
			_CurrentEntry->ListNext = _NewMemoryRangeEntry;
			_NewMemoryRangeEntry->ListPrev = _CurrentEntry;
			
			//adjust size
			_CurrentEntry->Size = _VirtualBase - (uintptr_t)_CurrentEntry;
		}

		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
}

void CPMM::SetHighFree(void *base, size_t size) {
	
	//Align base to PAGE_SIZE
	if ((uintptr_t)base & (PAGE_SIZE - 1)) {
		base = (void*)((uintptr_t)base + PAGE_SIZE);
		base = (void*)((uintptr_t)base & ~(PAGE_SIZE - 1));
	}
	
	//Align size to PAGE_SIZE
	if (size & (PAGE_SIZE - 1)) {
		size += PAGE_SIZE;
		size &= ~(PAGE_SIZE - 1);
	}
	
	// Ignore if below 4GB
	if (((uintptr_t)base + size) < MEMORY_LOW_END)
		return;
	
	// Cut Range below 4GB
	if ((uintptr_t)base < MEMORY_LOW_END) {
		size -= MEMORY_LOW_END - (uintptr_t)base;
		base = (void*)MEMORY_LOW_END;
	}
	
	//Check if Entry already exists
	MemoryRange_t *_CurrentEntry = mMemoryHighList;
	while (_CurrentEntry != NULL) {
		
		//Case: Base is the same
		if (_CurrentEntry == (MemoryRange_t*)((uintptr_t)base + (uintptr_t)Paging::GetHHDMOffset())) {
			// If new size is greater, then just update size
			if (size > _CurrentEntry->Size) {
				_CurrentEntry->Size = size;
				return;
			}
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	// Create MemoryRange Entry
	MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)base + (uintptr_t)Paging::GetHHDMOffset());
	_NewMemoryRangeEntry->Size = size;
	_NewMemoryRangeEntry->ListNext = NULL;
	_NewMemoryRangeEntry->ListPrev = NULL;
	
	// Add Entry to the list
	if (mMemoryHighList == NULL) {
		mMemoryHighList = _NewMemoryRangeEntry;
		return;
	} else {
		//Search List for Position to insert
		MemoryRange_t *_CurrentEntry = mMemoryHighList;
		
		while (_CurrentEntry != NULL) {
			
			// if found, add in between
			if ((uintptr_t)_CurrentEntry > (uintptr_t)_NewMemoryRangeEntry) {
				
				if (_CurrentEntry->ListPrev == NULL) {
					mMemoryHighList = _NewMemoryRangeEntry;
				} else {
					_CurrentEntry->ListPrev->ListNext = _NewMemoryRangeEntry;
					_NewMemoryRangeEntry->ListPrev = _CurrentEntry->ListPrev;
				}
				_CurrentEntry->ListPrev = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListNext = _CurrentEntry;
				break;
			}
			
			// if Last entry, add to back of list
			if (_CurrentEntry->ListNext == NULL)  {
				_CurrentEntry->ListNext = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListPrev = _CurrentEntry;				
				break;
			}
			
			_CurrentEntry = _CurrentEntry->ListNext;
		}
		
	}
	
	MergeHigh();
}

void CPMM::SetHighUsed(void *base, size_t size) {
	
	//Align base to PAGE_SIZE
	if ((uintptr_t)base & (PAGE_SIZE - 1)) {
		base = (void*)((uintptr_t)base + PAGE_SIZE);
		base = (void*)((uintptr_t)base & ~(PAGE_SIZE - 1));
	}
	
	//Align size to PAGE_SIZE
	if (size & (PAGE_SIZE - 1)) {
		size += PAGE_SIZE;
		size &= ~(PAGE_SIZE - 1);
	}
	
	// Ignore if below 4GB
	if (((uintptr_t)base + size) < MEMORY_LOW_END)
		return;
	
	
	
	uintptr_t _VirtualBase = (uintptr_t)base + (uintptr_t)Paging::GetHHDMOffset();
	
	// Loop over each Range
	MemoryRange_t *_CurrentEntry = mMemoryHighList;
	while (_CurrentEntry != NULL) {
		
		// Case: entire entry can be deleted
		if ((_VirtualBase <= (uintptr_t)_CurrentEntry) && 
			((_VirtualBase + size) >= ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
			
			if (_CurrentEntry->ListPrev == NULL) {
				mMemoryHighList = _CurrentEntry->ListNext;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = NULL;
				}
			} else {
				_CurrentEntry->ListPrev = _CurrentEntry->ListNext;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = _CurrentEntry->ListPrev;
				}
			}
		
		// Case: start of entry can be deleted
		} else if ((_VirtualBase <= (uintptr_t)_CurrentEntry) && 
			((_VirtualBase + size) > ((uintptr_t)_CurrentEntry)) &&
			((_VirtualBase + size) < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
		
			// Create MemoryRange Entry
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)base + size + (uintptr_t)Paging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (uintptr_t)_CurrentEntry + _CurrentEntry->Size - (uintptr_t)_NewMemoryRangeEntry;
			_NewMemoryRangeEntry->ListNext = NULL;
			_NewMemoryRangeEntry->ListPrev = NULL;
			
			// Replace _CurrentEntry with _NewMemoryRangeEntry
			if (_CurrentEntry->ListPrev == NULL) {
				mMemoryHighList = _NewMemoryRangeEntry;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = _NewMemoryRangeEntry;
					_NewMemoryRangeEntry->ListNext = _CurrentEntry->ListNext;
				}
			} else {
				_CurrentEntry->ListPrev->ListNext = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListPrev = _CurrentEntry->ListPrev;
				if (_CurrentEntry->ListNext != NULL) {
					_CurrentEntry->ListNext->ListPrev = _NewMemoryRangeEntry;
					_NewMemoryRangeEntry->ListNext = _CurrentEntry->ListNext;
				}
			}
		
		// Case: end of entry can be deleted
		} else if ((_VirtualBase > (uintptr_t)_CurrentEntry) && 
			((_VirtualBase) < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) &&
			((_VirtualBase + size) >= ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
	
			//Only adjust size
			_CurrentEntry->Size = _VirtualBase - (uintptr_t)_CurrentEntry;
			
			
		// Case: CurrentEntry needs to be split
		} else if ((_VirtualBase > (uintptr_t)_CurrentEntry) && 
			(_VirtualBase < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) && 
			((_VirtualBase + size) < ((uintptr_t)_CurrentEntry + _CurrentEntry->Size))) {
			
			//Create new Entry for the range at the end
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)base + size + (uintptr_t)Paging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (uintptr_t)_CurrentEntry + _CurrentEntry->Size - (uintptr_t)_NewMemoryRangeEntry;
			_NewMemoryRangeEntry->ListNext = NULL;
			_NewMemoryRangeEntry->ListPrev = NULL;
			
			
			// Insert _NewMemoryRangeEntry behind current Entry
			if (_CurrentEntry->ListNext != NULL) {
				_CurrentEntry->ListNext->ListPrev = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListNext = _CurrentEntry->ListNext;
			}	
			_CurrentEntry->ListNext = _NewMemoryRangeEntry;
			_NewMemoryRangeEntry->ListPrev = _CurrentEntry;
			
			//adjust size
			_CurrentEntry->Size = _VirtualBase - (uintptr_t)_CurrentEntry;
		}

		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
}

void CPMM::MergeISA(void) {
	
	MemoryRange_t *_CurrentEntry = mMemoryISAList;
	while ((_CurrentEntry != NULL) && (_CurrentEntry->ListNext != NULL)) {
		
		//Check if adjacent or overlapping
		if ((uintptr_t)_CurrentEntry->ListNext <= ((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) {
			
			//Update size only if next entry is not totally inside current entry
			if (((uintptr_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) >
				((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) {
			
				_CurrentEntry->Size = ((uintptr_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) - (uintptr_t)_CurrentEntry;
			}
			
			//Remove double Entry
			_CurrentEntry->ListNext = _CurrentEntry->ListNext->ListNext;
			if (_CurrentEntry->ListNext != NULL)
				_CurrentEntry->ListNext->ListPrev = _CurrentEntry;
			
			//Check if _CurrentEntry can be merged with more Entries.
			continue;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
}

void CPMM::MergeLow(void) {
	
	MemoryRange_t *_CurrentEntry = mMemoryLowList;
	while ((_CurrentEntry != NULL) && (_CurrentEntry->ListNext != NULL)) {
		
		//Check if adjacent or overlapping
		if ((uintptr_t)_CurrentEntry->ListNext <= ((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) {
			
			//Update size only if next entry is not totally inside current entry
			if (((uintptr_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) >
				((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) {
			
				_CurrentEntry->Size = ((uintptr_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) - (uintptr_t)_CurrentEntry;
			}
			
			//Remove double Entry
			_CurrentEntry->ListNext = _CurrentEntry->ListNext->ListNext;
			if (_CurrentEntry->ListNext != NULL)
				_CurrentEntry->ListNext->ListPrev = _CurrentEntry;
			
			//Check if _CurrentEntry can be merged with more Entries.
			continue;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}	
}

void CPMM::MergeHigh(void) {
	MemoryRange_t *_CurrentEntry = mMemoryHighList;
	while ((_CurrentEntry != NULL) && (_CurrentEntry->ListNext != NULL)) {
		
		//Check if adjacent or overlapping
		if ((uintptr_t)_CurrentEntry->ListNext <= ((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) {
			
			//Update size only if next entry is not totally inside current entry
			if (((uintptr_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) >
				((uintptr_t)_CurrentEntry + _CurrentEntry->Size)) {
			
				_CurrentEntry->Size = ((uintptr_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) - (uintptr_t)_CurrentEntry;
			}
			
			//Remove double Entry
			_CurrentEntry->ListNext = _CurrentEntry->ListNext->ListNext;
			if (_CurrentEntry->ListNext != NULL)
				_CurrentEntry->ListNext->ListPrev = _CurrentEntry;
			
			//Check if _CurrentEntry can be merged with more Entries.
			continue;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
}

void CPMM::SetFree(void *base, size_t size) {
	SetISAFree(base, size);
	SetLowFree(base, size);
	SetHighFree(base, size);
}

void CPMM::SetUsed(void *base, size_t size) {
	SetISAUsed(base, size);
	SetLowUsed(base, size);
	SetHighUsed(base, size);	
}

ReturnValue CPMM::Alloc(void **address, size_t size) {
	ReturnValue RetVal = RetValErrorGeneral;
	
	//Try allocating High Memory
	RetVal = AllocHigh(address, size);
	if (IS_SUCCESS(RetVal))
		return RetVal;
	
	//Try allocating Low Memory next
	RetVal = AllocLow(address, size);
	if (IS_SUCCESS(RetVal))
		return RetVal;
	
	//Try allocating ISA Memory as last resort
	RetVal = AllocISA(address, size);
	return RetVal;
}

ReturnValue CPMM::AllocISA(void **address, size_t size) {
	
	//Align size to PAGE_SIZE
	if (size & (PAGE_SIZE - 1)) {
		size += PAGE_SIZE;
		size &= ~(PAGE_SIZE - 1);
	}
	
	if (mMemoryISAList == NULL)
		return RETVAL_ERROR_OOM_PHYSICAL;
	
	MemoryRange_t *_CurrentEntry = mMemoryISAList;
	while (_CurrentEntry != NULL) {
		
		//Found Big enough entry
		if (_CurrentEntry->Size >= size) {
			
			//Allocate at end of _CurrentEntry - resulting in a resize only
			//If allocated at the start the entire Entry as well as Next and Prev would have to be updated
			*address = (void*)((uintptr_t)_CurrentEntry + _CurrentEntry->Size - (uintptr_t)CPaging::GetHHDMOffset() - size);
			SetUsed(*address, size);
			return RETVAL_OK;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	return RETVAL_ERROR_OOM_PHYSICAL;
}

ReturnValue CPMM::AllocLow(void **address, size_t size) {
	
	//Align size to PAGE_SIZE
	if (size & (PAGE_SIZE - 1)) {
		size += PAGE_SIZE;
		size &= ~(PAGE_SIZE - 1);
	}
	
	if (mMemoryISAList == NULL)
		return RETVAL_ERROR_OOM_PHYSICAL;
	
	MemoryRange_t *_CurrentEntry = mMemoryLowList;
	while (_CurrentEntry != NULL) {
		
		//Found Big enough entry
		if (_CurrentEntry->Size >= size) {
			
			//Allocate at end of _CurrentEntry - resulting in a resize only
			//If allocated at the start the entire Entry as well as Next and Prev would have to be updated
			*address = (void*)((uintptr_t)_CurrentEntry + _CurrentEntry->Size - (uintptr_t)CPaging::GetHHDMOffset() - size);
			SetUsed(*address, size);
			return RETVAL_OK;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	return RETVAL_ERROR_OOM_PHYSICAL;
}

ReturnValue CPMM::AllocHigh(void **address, size_t size) {
	
	//Align size to PAGE_SIZE
	if (size & (PAGE_SIZE - 1)) {
		size += PAGE_SIZE;
		size &= ~(PAGE_SIZE - 1);
	}
	
	if (mMemoryISAList == NULL)
		return RETVAL_ERROR_OOM_PHYSICAL;
	
	MemoryRange_t *_CurrentEntry = mMemoryHighList;
	while (_CurrentEntry != NULL) {
		
		//Found Big enough entry
		if (_CurrentEntry->Size >= size) {
			
			//Allocate at end of _CurrentEntry - resulting in a resize only
			//If allocated at the start the entire Entry as well as Next and Prev would have to be updated
			*address = (void*)((uintptr_t)_CurrentEntry + _CurrentEntry->Size - (uintptr_t)CPaging::GetHHDMOffset() - size);
			SetUsed(*address, size);
			return RETVAL_OK;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	return RETVAL_ERROR_OOM_PHYSICAL;
}

void CPMM::Free(void *address, size_t size) {
	SetFree(address, size);
}

