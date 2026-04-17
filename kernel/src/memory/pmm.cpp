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

uint64_t CPMM::mFreeMemoryAmount = 0;
uint64_t CPMM::mUsedMemoryAmount = 0;


ReturnValue_t CPMM::PreInit(void) {

	limine_memmap_response *_LimineMemoryMapResponse = CLimine::GetMemoryMapResponse();
	uint64_t _UsedMemoryAmount = 0;
	uint64_t _FreeMemoryAmount = 0;



	// Set ISA Memory (below 1MB)
	for (size_t i = 0; i < _LimineMemoryMapResponse->entry_count ; ++i) {
		limine_memmap_entry *_LimineMemoryMapEntry = _LimineMemoryMapResponse->entries[i];
		
		// Skip not usable memory
		if (_LimineMemoryMapEntry->type == LIMINE_MEMMAP_USABLE) {
			SetISAFree((PhysicalAddress_t)_LimineMemoryMapEntry->base, _LimineMemoryMapEntry->length);	
			SetLowFree((PhysicalAddress_t)_LimineMemoryMapEntry->base, _LimineMemoryMapEntry->length);
			SetHighFree((PhysicalAddress_t)_LimineMemoryMapEntry->base, _LimineMemoryMapEntry->length);
			_FreeMemoryAmount += _LimineMemoryMapEntry->length;
		} else if (_LimineMemoryMapEntry->type == LIMINE_MEMMAP_EXECUTABLE_AND_MODULES) {
			_UsedMemoryAmount += _LimineMemoryMapEntry->length;
		}
		

	}
	
	mUsedMemoryAmount = _UsedMemoryAmount;
	mFreeMemoryAmount = _FreeMemoryAmount;

	if (mMemoryISAList == NULL)
		return RETVAL_ERROR_GENERAL;
	
	return RETVAL_OK;
}

ReturnValue_t CPMM::Init(void) {
	
	return RETVAL_ERROR_GENERAL;
}


void CPMM::PrintMemoryMap(void) {
	
	CLog::Print("MemoryMap:\n");
	CLog::Print("Start              | End                | Size              \n");
	CLog::Print("------------------------------------------------------------\n");
	
	MemoryRange_t *_CurrentRange = NULL;
	
	if (mMemoryISAList != NULL) {
		_CurrentRange = mMemoryISAList;
		while (_CurrentRange != NULL) {
			
			CLog::PrintF("%016p | %016p | %016p\n", (PhysicalAddress_t)_CurrentRange - (PhysicalAddress_t)CPaging::GetHHDMOffset(), 
				(PhysicalAddress_t)_CurrentRange - (PhysicalAddress_t)CPaging::GetHHDMOffset() + _CurrentRange->Size - 1,
				_CurrentRange->Size);
			_CurrentRange = _CurrentRange->ListNext;
			
		}
	}

	if (mMemoryLowList != NULL) {
		_CurrentRange = mMemoryLowList;
		while (_CurrentRange != NULL) {
			
			CLog::PrintF("%016p | %016p | %016p\n", (PhysicalAddress_t)_CurrentRange - (PhysicalAddress_t)CPaging::GetHHDMOffset(), 
				(PhysicalAddress_t)_CurrentRange - (PhysicalAddress_t)CPaging::GetHHDMOffset() + _CurrentRange->Size - 1,
				_CurrentRange->Size);
			_CurrentRange = _CurrentRange->ListNext;
			
		}
	}
	
	if (mMemoryHighList != NULL) {
		_CurrentRange = mMemoryHighList;
		while (_CurrentRange != NULL) {
			
			CLog::PrintF("%016p | %016p | %016p\n", (PhysicalAddress_t)_CurrentRange - (PhysicalAddress_t)CPaging::GetHHDMOffset(), 
				(PhysicalAddress_t)_CurrentRange - (PhysicalAddress_t)CPaging::GetHHDMOffset() + _CurrentRange->Size - 1,
				_CurrentRange->Size);
			_CurrentRange = _CurrentRange->ListNext;
			
		}
	}
}

void CPMM::SetISAFree(PhysicalAddress_t pBase, size_t pSize) {

	//Align pBase to PAGE_SIZE
	if (pBase & (PAGE_SIZE - 1)) {
		pBase = pBase + PAGE_SIZE;
		pBase = pBase & ~(PAGE_SIZE - 1);
	}
	
	//Align pSize to PAGE_SIZE
	if (pSize & (PAGE_SIZE - 1)) {
		pSize += PAGE_SIZE;
		pSize &= ~(PAGE_SIZE - 1);
	}
	
	// Ignore if above 1MB
	if (pBase >= MEMORY_ISA_END)
		return;
	
	// Cut Range above 1MB
	if ((pBase + pSize) > MEMORY_ISA_END) {
		pSize = MEMORY_ISA_END - pBase;
	}

	//Check if Entry already exists
	MemoryRange_t *_CurrentEntry = mMemoryISAList;
	while (_CurrentEntry != NULL) {
		
		//Case: Base is the same
		if (_CurrentEntry == (MemoryRange_t*)(pBase + CPaging::GetHHDMOffset())) {
			// If new size is greater, then just update size
			if (pSize > _CurrentEntry->Size) {
				_CurrentEntry->Size = pSize;
				return;
			}
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	// Create MemoryRange Entry
	MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)(pBase + CPaging::GetHHDMOffset());
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
			if ((PhysicalAddress_t)_CurrentEntry > (PhysicalAddress_t)_NewMemoryRangeEntry) {
				
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

void CPMM::SetISAUsed(PhysicalAddress_t pBase, size_t pSize) {

	//Align pBase to PAGE_SIZE
	if (pBase & (PAGE_SIZE - 1)) {
		pBase = pBase + PAGE_SIZE;
		pBase = pBase & ~(PAGE_SIZE - 1);
	}
	
	//Align pSize to PAGE_SIZE
	if (pSize & (PAGE_SIZE - 1)) {
		pSize += PAGE_SIZE;
		pSize &= ~(PAGE_SIZE - 1);
	}
	
	// if there is no list we can directly return
	if (mMemoryISAList == NULL)
		return;

	// Ignore if above 1MB
	if ((uintptr_t)pBase >= MEMORY_ISA_END)
		return;
	
	PhysicalAddress_t _VirtualBase = pBase + CPaging::GetHHDMOffset();
	
	// Loop over each Range
	MemoryRange_t *_CurrentEntry = mMemoryISAList;
	while (_CurrentEntry != NULL) {
		
		// Case: entire entry can be deleted
		if ((_VirtualBase <= (PhysicalAddress_t)_CurrentEntry) && 
			((PhysicalAddress_t)(_VirtualBase + pSize) >= (PhysicalAddress_t)(_CurrentEntry + _CurrentEntry->Size))) {
			
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
		} else if ((_VirtualBase <= (PhysicalAddress_t)_CurrentEntry) && 
			((PhysicalAddress_t)(_VirtualBase + pSize) > (PhysicalAddress_t)_CurrentEntry) &&
			((PhysicalAddress_t)(_VirtualBase + pSize) < (PhysicalAddress_t)(((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)))) {
		
			// Create MemoryRange Entry
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)(pBase + pSize + CPaging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size - (PhysicalAddress_t)_NewMemoryRangeEntry;
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
		} else if ((_VirtualBase > (PhysicalAddress_t)_CurrentEntry) && 
			((PhysicalAddress_t)(_VirtualBase) < (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) &&
			((PhysicalAddress_t)(_VirtualBase + pSize) >= (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size))) {
	
			//Only adjust size
			_CurrentEntry->Size = _VirtualBase - (PhysicalAddress_t)_CurrentEntry;
			
			
		// Case: CurrentEntry needs to be split
		} else if ((_VirtualBase > (PhysicalAddress_t)_CurrentEntry) && 
			(_VirtualBase < (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) && 
			((PhysicalAddress_t)(_VirtualBase + pSize) < (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size))) {
			
			//Create new Entry for the range at the end
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)(pBase + pSize + CPaging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size - (PhysicalAddress_t)_NewMemoryRangeEntry;
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
			_CurrentEntry->Size = _VirtualBase - (PhysicalAddress_t)_CurrentEntry;
		}

		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
}

void CPMM::SetLowFree(PhysicalAddress_t pBase, size_t pSize) {
	
	//Align pBase to PAGE_SIZE
	if (pBase & (PAGE_SIZE - 1)) {
		pBase = pBase + PAGE_SIZE;
		pBase = pBase & ~(PAGE_SIZE - 1);
	}
	
	//Align pSize to PAGE_SIZE
	if (pSize & (PAGE_SIZE - 1)) {
		pSize += PAGE_SIZE;
		pSize &= ~(PAGE_SIZE - 1);
	}
	
	// Ignore if above 4GB
	if (pBase >= MEMORY_LOW_END)
		return;

	// Ignore below 1MB
	if ((pBase + pSize) < MEMORY_ISA_END)
		return;
	
	// Cut Range below 1MB
	if (pBase < MEMORY_ISA_END) {
		pSize -= MEMORY_ISA_END - pBase;
		pBase = (PhysicalAddress_t)MEMORY_ISA_END;
	}
	
	// Cut Range above 4GB
	if ((pBase + pSize) > MEMORY_LOW_END) {
		pSize = MEMORY_LOW_END - pBase;
	}

	//Check if Entry already exists
	MemoryRange_t *_CurrentEntry = mMemoryLowList;
	while (_CurrentEntry != NULL) {
		
		//Case: Base is the same
		if (_CurrentEntry == (MemoryRange_t*)(pBase + CPaging::GetHHDMOffset())) {
			// If new size is greater, then just update size
			if (pSize > _CurrentEntry->Size) {
				_CurrentEntry->Size = pSize;
				return;
			}
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	// Create MemoryRange Entry
	MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)(pBase + CPaging::GetHHDMOffset());
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
			if ((PhysicalAddress_t)_CurrentEntry > (PhysicalAddress_t)_NewMemoryRangeEntry) {
			
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

void CPMM::SetLowUsed(PhysicalAddress_t pBase, size_t pSize) {
	
	//Align pBase to PAGE_SIZE
	if (pBase & (PAGE_SIZE - 1)) {
		pBase = pBase + PAGE_SIZE;
		pBase = pBase & ~(PAGE_SIZE - 1);
	}
	
	//Align pSize to PAGE_SIZE
	if (pSize & (PAGE_SIZE - 1)) {
		pSize += PAGE_SIZE;
		pSize &= ~(PAGE_SIZE - 1);
	}
	
	// Ignore if above 4GB
	if (pBase >= MEMORY_LOW_END)
		return;

	// Ignore below 1MB
	if ((pBase + pSize) < MEMORY_ISA_END)
		return;
	
	
	PhysicalAddress_t _VirtualBase = pBase + CPaging::GetHHDMOffset();
	
	// Loop over each Range
	MemoryRange_t *_CurrentEntry = mMemoryLowList;
	while (_CurrentEntry != NULL) {
		
		// Case: entire entry can be deleted
		if ((_VirtualBase <= (PhysicalAddress_t)_CurrentEntry) && 
			((PhysicalAddress_t)(_VirtualBase + pSize) >= (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size))) {
			
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
		} else if ((_VirtualBase <= (PhysicalAddress_t)_CurrentEntry) && 
			((PhysicalAddress_t)(_VirtualBase + pSize) > (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry)) &&
			((PhysicalAddress_t)(_VirtualBase + pSize) < (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size))) {
		
			// Create MemoryRange Entry
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)(pBase + pSize + CPaging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size - (PhysicalAddress_t)_NewMemoryRangeEntry;
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
		} else if ((_VirtualBase > (PhysicalAddress_t)_CurrentEntry) && 
			((PhysicalAddress_t)(_VirtualBase) < (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) &&
			((PhysicalAddress_t)(_VirtualBase + pSize) >= (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size))) {
	
			//Only adjust size
			_CurrentEntry->Size = _VirtualBase - (PhysicalAddress_t)_CurrentEntry;
			
			
		// Case: CurrentEntry needs to be split
		} else if ((_VirtualBase > (PhysicalAddress_t)_CurrentEntry) && 
			(_VirtualBase < (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) && 
			((PhysicalAddress_t)(_VirtualBase + pSize) < (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size))) {
			
			//Create new Entry for the range at the end
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)(pBase + pSize + CPaging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size - (PhysicalAddress_t)_NewMemoryRangeEntry;
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
			_CurrentEntry->Size = _VirtualBase - (PhysicalAddress_t)_CurrentEntry;
		}

		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
}

void CPMM::SetHighFree(PhysicalAddress_t pBase, size_t pSize) {
	
	//Align pBase to PAGE_SIZE
	if (pBase & (PAGE_SIZE - 1)) {
		pBase = pBase + PAGE_SIZE;
		pBase = pBase & ~(PAGE_SIZE - 1);
	}
	
	//Align pSize to PAGE_SIZE
	if (pSize & (PAGE_SIZE - 1)) {
		pSize += PAGE_SIZE;
		pSize &= ~(PAGE_SIZE - 1);
	}
	
	// Ignore if below 4GB
	if ((pBase + pSize) < MEMORY_LOW_END)
		return;
	
	// Cut Range below 4GB
	if (pBase < MEMORY_LOW_END) {
		pSize -= MEMORY_LOW_END - pBase;
		pBase = (PhysicalAddress_t)MEMORY_LOW_END;
	}
	
	//Check if Entry already exists
	MemoryRange_t *_CurrentEntry = mMemoryHighList;
	while (_CurrentEntry != NULL) {
		
		//Case: Base is the same
		if (_CurrentEntry == (MemoryRange_t*)(pBase + CPaging::GetHHDMOffset())) {
			// If new size is greater, then just update size
			if (pSize > _CurrentEntry->Size) {
				_CurrentEntry->Size = pSize;
				return;
			}
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	// Create MemoryRange Entry
	MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)(pBase + CPaging::GetHHDMOffset());
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
			if ((PhysicalAddress_t)_CurrentEntry > (PhysicalAddress_t)_NewMemoryRangeEntry) {
				
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

void CPMM::SetHighUsed(PhysicalAddress_t pBase, size_t pSize) {
	
	//Align pBase to PAGE_SIZE
	if (pBase & (PAGE_SIZE - 1)) {
		pBase = pBase + PAGE_SIZE;
		pBase = pBase & ~(PAGE_SIZE - 1);
	}
	
	//Align pSize to PAGE_SIZE
	if (pSize & (PAGE_SIZE - 1)) {
		pSize += PAGE_SIZE;
		pSize &= ~(PAGE_SIZE - 1);
	}
	
	// Ignore if below 4GB
	if ((pBase + pSize) < MEMORY_LOW_END)
		return;
	
	
	
	PhysicalAddress_t _VirtualBase = pBase + CPaging::GetHHDMOffset();
	
	// Loop over each Range
	MemoryRange_t *_CurrentEntry = mMemoryHighList;
	while (_CurrentEntry != NULL) {
		
		// Case: entire entry can be deleted
		if ((_VirtualBase <= (PhysicalAddress_t)_CurrentEntry) && 
			((_VirtualBase + pSize) >= ((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size))) {
			
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
		} else if ((_VirtualBase <= (PhysicalAddress_t)_CurrentEntry) && 
			((PhysicalAddress_t)(_VirtualBase + pSize) > (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry)) &&
			((PhysicalAddress_t)(_VirtualBase + pSize) < (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size))) {
		
			// Create MemoryRange Entry
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)(pBase + pSize + CPaging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size - (PhysicalAddress_t)_NewMemoryRangeEntry;
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
		} else if ((_VirtualBase > (PhysicalAddress_t)_CurrentEntry) && 
			((PhysicalAddress_t)(_VirtualBase) < (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) &&
			((PhysicalAddress_t)(_VirtualBase + pSize) >= (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size))) {
	
			//Only adjust size
			_CurrentEntry->Size = _VirtualBase - (PhysicalAddress_t)_CurrentEntry;
			
			
		// Case: CurrentEntry needs to be split
		} else if ((_VirtualBase > (PhysicalAddress_t)_CurrentEntry) && 
			(_VirtualBase < (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) && 
			((PhysicalAddress_t)(_VirtualBase + pSize) < (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size))) {
			
			//Create new Entry for the range at the end
			MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)(pBase + pSize + CPaging::GetHHDMOffset());
			_NewMemoryRangeEntry->Size = (PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size - (PhysicalAddress_t)_NewMemoryRangeEntry;
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
			_CurrentEntry->Size = _VirtualBase - (PhysicalAddress_t)_CurrentEntry;
		}

		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
}

void CPMM::MergeISA(void) {
	
	MemoryRange_t *_CurrentEntry = mMemoryISAList;
	while ((_CurrentEntry != NULL) && (_CurrentEntry->ListNext != NULL)) {
		
		//Check if adjacent or overlapping
		if ((PhysicalAddress_t)_CurrentEntry->ListNext <= (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) {
			
			//Update size only if next entry is not totally inside current entry
			if ((PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) >
				(PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) {
			
				_CurrentEntry->Size = ((PhysicalAddress_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) - (PhysicalAddress_t)_CurrentEntry;
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
		if ((PhysicalAddress_t)_CurrentEntry->ListNext <= (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) {
			
			//Update size only if next entry is not totally inside current entry
			if ((PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) >
				(PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) {
			
				_CurrentEntry->Size = ((PhysicalAddress_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) - (PhysicalAddress_t)_CurrentEntry;
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
		if ((PhysicalAddress_t)_CurrentEntry->ListNext <= (PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) {
			
			//Update size only if next entry is not totally inside current entry
			if ((PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) >
				(PhysicalAddress_t)((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size)) {
			
				_CurrentEntry->Size = ((PhysicalAddress_t)_CurrentEntry->ListNext + _CurrentEntry->ListNext->Size) - (PhysicalAddress_t)_CurrentEntry;
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

void CPMM::SetFree(PhysicalAddress_t pBase, size_t pSize) {
	SetISAFree(pBase, pSize);
	SetLowFree(pBase, pSize);
	SetHighFree(pBase, pSize);
}

void CPMM::SetUsed(PhysicalAddress_t pBase, size_t pSize) {
	SetISAUsed(pBase, pSize);
	SetLowUsed(pBase, pSize);
	SetHighUsed(pBase, pSize);	
}

ReturnValue_t CPMM::Alloc(PhysicalAddress_t &pAddress, size_t pSize) {
	ReturnValue_t _RetVal = RETVAL_ERROR_GENERAL;
	
	//Try allocating High Memory
	_RetVal = AllocHigh(pAddress, pSize);
	if (IS_SUCCESS(_RetVal))
		return _RetVal;
	
	//Try allocating Low Memory next
	_RetVal = AllocLow(pAddress, pSize);
	if (IS_SUCCESS(_RetVal))
		return _RetVal;
	
	//Try allocating ISA Memory as last resort
	_RetVal = AllocISA(pAddress, pSize);
	return _RetVal;
}

ReturnValue_t CPMM::AllocISA(PhysicalAddress_t &pAddress, size_t pSize) {
	
	//Align pSize to PAGE_SIZE
	if (pSize & (PAGE_SIZE - 1)) {
		pSize += PAGE_SIZE;
		pSize &= ~(PAGE_SIZE - 1);
	}
	
	if (mMemoryISAList == NULL)
		return RETVAL_ERROR_OOM_PHYSICAL;
	
	MemoryRange_t *_CurrentEntry = mMemoryISAList;
	while (_CurrentEntry != NULL) {
		
		//Found Big enough entry
		if (_CurrentEntry->Size >= pSize) {
			
			//Allocate at end of _CurrentEntry - resulting in a resize only
			//If allocated at the start the entire Entry as well as Next and Prev would have to be updated
			pAddress = ((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size - CPaging::GetHHDMOffset() - pSize);
			SetUsed(pAddress, pSize);
			return RETVAL_OK;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	return RETVAL_ERROR_OOM_PHYSICAL;
}

ReturnValue_t CPMM::AllocLow(PhysicalAddress_t &pAddress, size_t pSize) {
	
	//Align pSize to PAGE_SIZE
	if (pSize & (PAGE_SIZE - 1)) {
		pSize += PAGE_SIZE;
		pSize &= ~(PAGE_SIZE - 1);
	}
	
	if (mMemoryISAList == NULL)
		return RETVAL_ERROR_OOM_PHYSICAL;
	
	MemoryRange_t *_CurrentEntry = mMemoryLowList;
	while (_CurrentEntry != NULL) {
		
		//Found Big enough entry
		if (_CurrentEntry->Size >= pSize) {
			
			//Allocate at end of _CurrentEntry - resulting in a resize only
			//If allocated at the start the entire Entry as well as Next and Prev would have to be updated
			pAddress = ((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size - CPaging::GetHHDMOffset() - pSize);
			SetUsed(pAddress, pSize);
			return RETVAL_OK;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	return RETVAL_ERROR_OOM_PHYSICAL;
}

ReturnValue_t CPMM::AllocHigh(PhysicalAddress_t &pAddress, size_t pSize) {
	
	//Align pSize to PAGE_SIZE
	if (pSize & (PAGE_SIZE - 1)) {
		pSize += PAGE_SIZE;
		pSize &= ~(PAGE_SIZE - 1);
	}
	
	if (mMemoryISAList == NULL)
		return RETVAL_ERROR_OOM_PHYSICAL;
	
	MemoryRange_t *_CurrentEntry = mMemoryHighList;
	while (_CurrentEntry != NULL) {
		
		//Found Big enough entry
		if (_CurrentEntry->Size >= pSize) {
			
			//Allocate at end of _CurrentEntry - resulting in a resize only
			//If allocated at the start the entire Entry as well as Next and Prev would have to be updated
			pAddress = ((PhysicalAddress_t)_CurrentEntry + _CurrentEntry->Size - CPaging::GetHHDMOffset() - pSize);
			SetUsed(pAddress, pSize);
			return RETVAL_OK;
		}
		
		_CurrentEntry = _CurrentEntry->ListNext;
	}
	
	return RETVAL_ERROR_OOM_PHYSICAL;
}

void CPMM::Free(PhysicalAddress_t pAddress, size_t pSize) {
	SetFree(pAddress, pSize);
}

