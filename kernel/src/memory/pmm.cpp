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
	for (size_t i = 0; i < _LimineMemoryMapResponse->entry_count; ++i) {
		limine_memmap_entry *_LimineMemoryMapEntry = _LimineMemoryMapResponse->entries[i];
		
		// Skip not usable memory
		if (_LimineMemoryMapEntry->type != LIMINE_MEMMAP_USABLE)
			continue;
		
		// Skip if Memory starts above 1MB
		if (_LimineMemoryMapEntry->base >= MEMORY_ISA_END)
			continue;
		
		// Create MemoryRange Entry
		MemoryRange_t *_NewMemoryRangeEntry = (MemoryRange_t*)(_LimineMemoryMapEntry->base + (uintptr_t)CPaging::GetHHDMOffset());
		_NewMemoryRangeEntry->Size = _LimineMemoryMapEntry->length;
		_NewMemoryRangeEntry->ListNext = NULL;
		_NewMemoryRangeEntry->ListPrev = NULL;
		
		// Cut size over 1MB
		if (((uintptr_t)_NewMemoryRangeEntry - (uintptr_t)CPaging::GetHHDMOffset() + _NewMemoryRangeEntry->Size) > MEMORY_ISA_END) {
			_NewMemoryRangeEntry->Size = MEMORY_ISA_END - ((uintptr_t)_NewMemoryRangeEntry - (uintptr_t)CPaging::GetHHDMOffset());
		}
		
		
		// Add Entry to the list
		if (mMemoryISAList == NULL) {
			mMemoryISAList = _NewMemoryRangeEntry;
		} else {
			//Search for end of list
			MemoryRange_t *_LastEntry = mMemoryISAList;
			while (_LastEntry->ListNext != NULL)
				_LastEntry = _LastEntry->ListNext;
			
			//Add to end of list
			_LastEntry->ListNext = _NewMemoryRangeEntry;
			_NewMemoryRangeEntry->ListPrev = _LastEntry;
		}

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
