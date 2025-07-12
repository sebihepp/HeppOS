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

MemoryRange_t *CPMM::mMemoryISAList = NULL;
MemoryRange_t *CPMM::mMemoryLowList = NULL;
MemoryRange_t *CPMM::mMemoryHighList = NULL;


ReturnValue_t CPMM::PreInit(void) {

	limine_memmap_response *_LimineMemoryMapResponse = CLimine::GetMemoryMapResponse();

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
		return RETVAL_ERROR_GENERAL;
	
	return RETVAL_OK;
}

void CPMM::PrintMemoryMap(void) {
	
	CLog::Print("MemoryMap:\n");
	CLog::Print("Start              | End                | Size              \n");
	CLog::Print("------------------------------------------------------------\n");
	
	MemoryRange_t *_CurrentRange = NULL;
	
	if (mMemoryISAList != NULL) {
		_CurrentRange = mMemoryISAList;
		while (_CurrentRange != NULL) {
			
			CLog::PrintF("0x%016lX | 0x%016lX | 0x%016lX\n", (uintptr_t)_CurrentRange - (uintptr_t)CPaging::GetHHDMOffset(), 
				(uintptr_t)_CurrentRange - (uintptr_t)CPaging::GetHHDMOffset() + _CurrentRange->Size - 1,
				_CurrentRange->Size);
			_CurrentRange = _CurrentRange->ListNext;
			
		}
	}

	if (mMemoryLowList != NULL) {
		_CurrentRange = mMemoryLowList;
		while (_CurrentRange != NULL) {
			
			CLog::PrintF("0x%016lX | 0x%016lX | 0x%016lX\n", (uintptr_t)_CurrentRange - (uintptr_t)CPaging::GetHHDMOffset(), 
				(uintptr_t)_CurrentRange - (uintptr_t)CPaging::GetHHDMOffset() + _CurrentRange->Size - 1,
				_CurrentRange->Size);
			_CurrentRange = _CurrentRange->ListNext;
			
		}
	}
	
	if (mMemoryHighList != NULL) {
		_CurrentRange = mMemoryHighList;
		while (_CurrentRange != NULL) {
			
			CLog::PrintF("0x%016lX | 0x%016lX | 0x%016lX\n", (uintptr_t)_CurrentRange - (uintptr_t)CPaging::GetHHDMOffset(), 
				(uintptr_t)_CurrentRange - (uintptr_t)CPaging::GetHHDMOffset() + _CurrentRange->Size - 1,
				_CurrentRange->Size);
			_CurrentRange = _CurrentRange->ListNext;
			
		}
	}
}

ReturnValue_t CPMM::Init(void) {
	
	return RETVAL_ERROR_GENERAL;
}

void CPMM::SetISAFree(void *pBase, size_t pSize) {

	// Ignore if above 1MB
	if ((uintptr_t)pBase >= MEMORY_ISA_END)
		return;
	
	// Cut Range above 1MB
	if (((uintptr_t)pBase + pSize) > MEMORY_ISA_END) {
		pSize = MEMORY_ISA_END - (uintptr_t)pBase;
	}

	//Check if Entry already exists
	MemoryRange_t *_CurrentEntry = mMemoryISAList;
	while (_CurrentEntry != NULL) {
		
		//Case: Base is the same
		if (_CurrentEntry == (MemoryRange_t*)((uintptr_t)pBase + (uintptr_t)CPaging::GetHHDMOffset())) {
			// If new size is greater, then just update size
			if (pSize > _CurrentEntry->Size) {
				_CurrentEntry->Size = pSize;
				return;
			}
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	// Create MemoryRange Entry
	MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)pBase + (uintptr_t)CPaging::GetHHDMOffset());
	_NewMemoryRangeEntry->Size = pSize;
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
			}
			
			// if Last entry, add to back of list
			if (_CurrentEntry->ListNext == NULL)  {
				_CurrentEntry->ListNext = _NewMemoryRangeEntry;
				_NewMemoryRangeEntry->ListPrev = _CurrentEntry;				
				return;
			}
			
			_CurrentEntry = _CurrentEntry->ListNext;
		}
		
	}
	
	MergeISA();
	
}

void CPMM::SetISAUsed(void *pBase, size_t pSize) {
	
}

void CPMM::SetLowFree(void *pBase, size_t pSize) {
	
	// Ignore if above 4GB
	if ((uintptr_t)pBase >= MEMORY_LOW_END)
		return;

	// Ignore below 1MB
	if (((uintptr_t)pBase + (uintptr_t)pSize) < MEMORY_ISA_END)
		return;
	
	// Cut Range below 1MB
	if ((uintptr_t)pBase < MEMORY_ISA_END) {
		pSize -= MEMORY_ISA_END - (uintptr_t)pBase;
		pBase = (void*)MEMORY_ISA_END;
	}
	
	// Cut Range above 4GB
	if (((uintptr_t)pBase + pSize) > MEMORY_LOW_END) {
		pSize = MEMORY_LOW_END - (uintptr_t)pBase;
	}

	//Check if Entry already exists
	MemoryRange_t *_CurrentEntry = mMemoryLowList;
	while (_CurrentEntry != NULL) {
		
		//Case: Base is the same
		if (_CurrentEntry == (MemoryRange_t*)((uintptr_t)pBase + (uintptr_t)CPaging::GetHHDMOffset())) {
			// If new size is greater, then just update size
			if (pSize > _CurrentEntry->Size) {
				_CurrentEntry->Size = pSize;
				return;
			}
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	// Create MemoryRange Entry
	MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)pBase + (uintptr_t)CPaging::GetHHDMOffset());
	_NewMemoryRangeEntry->Size = pSize;
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
				return;
			}
			
			_CurrentEntry = _CurrentEntry->ListNext;
		}
		
	}
	
	MergeLow();
	
}

void CPMM::SetLowUsed(void *pBase, size_t pSize) {
	
}

void CPMM::SetHighFree(void *pBase, size_t pSize) {
		
	// Ignore if below 4GB
	if (((uintptr_t)pBase + pSize) < MEMORY_LOW_END)
		return;
	
	// Cut Range below 4GB
	if ((uintptr_t)pBase < MEMORY_LOW_END) {
		pSize -= MEMORY_LOW_END - (uintptr_t)pBase;
		pBase = (void*)MEMORY_LOW_END;
	}
	
	//Check if Entry already exists
	MemoryRange_t *_CurrentEntry = mMemoryHighList;
	while (_CurrentEntry != NULL) {
		
		//Case: Base is the same
		if (_CurrentEntry == (MemoryRange_t*)((uintptr_t)pBase + (uintptr_t)CPaging::GetHHDMOffset())) {
			// If new size is greater, then just update size
			if (pSize > _CurrentEntry->Size) {
				_CurrentEntry->Size = pSize;
				return;
			}
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	// Create MemoryRange Entry
	MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)((uintptr_t)pBase + (uintptr_t)CPaging::GetHHDMOffset());
	_NewMemoryRangeEntry->Size = pSize;
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
				return;
			}
			
			_CurrentEntry = _CurrentEntry->ListNext;
		}
		
	}
	
	MergeHigh();
}

void CPMM::SetHighUsed(void *pBase, size_t pSize) {
	
}

void CPMM::MergeISA(void) {
	MemoryRange_t *_CurrentEntry = mMemoryISAList;
	while ((_CurrentEntry != NULL) && (_CurrentEntry->ListNext != NULL)) {
		
		//Check if adjacent
		if (((uintptr_t)_CurrentEntry + _CurrentEntry->Size) == (uintptr_t)_CurrentEntry->ListNext) {
			
			//Update Size
			_CurrentEntry->Size += _CurrentEntry->ListNext->Size;
			
			//Remove double Entry
			_CurrentEntry->ListNext = _CurrentEntry->ListNext->ListNext;
			if (_CurrentEntry->ListNext != NULL)
				_CurrentEntry->ListNext->ListPrev = _CurrentEntry;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
}

void CPMM::MergeLow(void) {
	MemoryRange_t *_CurrentEntry = mMemoryLowList;
	while ((_CurrentEntry != NULL) && (_CurrentEntry->ListNext != NULL)) {
		
		//Check if adjacent
		if (((uintptr_t)_CurrentEntry + _CurrentEntry->Size) == (uintptr_t)_CurrentEntry->ListNext) {
			
			//Update Size
			_CurrentEntry->Size += _CurrentEntry->ListNext->Size;
			
			//Remove double Entry
			_CurrentEntry->ListNext = _CurrentEntry->ListNext->ListNext;
			if (_CurrentEntry->ListNext != NULL)
				_CurrentEntry->ListNext->ListPrev = _CurrentEntry;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}	
}

void CPMM::MergeHigh(void) {
	MemoryRange_t *_CurrentEntry = mMemoryHighList;
	while ((_CurrentEntry != NULL) && (_CurrentEntry->ListNext != NULL)) {
		
		//Check if adjacent
		if (((uintptr_t)_CurrentEntry + _CurrentEntry->Size) == (uintptr_t)_CurrentEntry->ListNext) {
			
			//Update Size
			_CurrentEntry->Size += _CurrentEntry->ListNext->Size;
			
			//Remove double Entry
			_CurrentEntry->ListNext = _CurrentEntry->ListNext->ListNext;
			if (_CurrentEntry->ListNext != NULL)
				_CurrentEntry->ListNext->ListPrev = _CurrentEntry;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
}
