
#include <memory/paging.h>
#include <liminestub.h>
#include <video/console.h>


// For quick testing - needs to be put in a string.h or something similar
char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);
char *htoa(uint64_t num, char* str);


bool CPaging::IsInitial = true;

static ReturnValue_t GetInitialPhysicalAddress(void *pVirtualAddress, void **pPhysicalAddress) {
	
	char _TempText[24];
	
	uint32_t _PML5Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 48) & 0x1FF;
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML5Index=");
	CConsole::Print(itoa(_PML5Index, _TempText, 10));
	CConsole::Print("\n"); */	
	uint32_t _PML4Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 39) & 0x1FF;
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML4Index=");
	CConsole::Print(itoa(_PML4Index, _TempText, 10));
	CConsole::Print("\n"); */
	uint32_t _PML3Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 30) & 0x1FF;
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML3Index=");
	CConsole::Print(itoa(_PML3Index, _TempText, 10));
	CConsole::Print("\n"); */
	uint32_t _PML2Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 21) & 0x1FF;
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML2Index=");
	CConsole::Print(itoa(_PML2Index, _TempText, 10));
	CConsole::Print("\n"); */
	uint32_t _PML1Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 12) & 0x1FF;
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML1Index=");
	CConsole::Print(itoa(_PML1Index, _TempText, 10));
	CConsole::Print("\n"); */


	
	volatile PML4_t *_PML4 = reinterpret_cast<volatile PML4_t*>(reinterpret_cast<uintptr_t>(CPaging::GetCR3()) + CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML4=0x");
	CConsole::Print(htoa((uint64_t)_PML4, _TempText));
	CConsole::Print("\n"); */
	if (_PML4->Entry[_PML4Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	
	
	volatile PML3_t *_PML3 = reinterpret_cast<volatile PML3_t*>((reinterpret_cast<uintptr_t>(_PML4->Entry[_PML4Index].Address) << 12) +
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML3=0x");
	CConsole::Print(htoa((uint64_t)_PML3, _TempText));
	CConsole::Print("\n"); */
	if (_PML3->Entry[_PML3Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}

	if (_PML3->Entry[_PML3Index].PageSize) {
		// CConsole::Print("CPaging::GetInitialPhysicalAddress() - 1G page size\n");
		*pPhysicalAddress = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_PML3->Entry1G[_PML3Index].Address) << 12) &
			(reinterpret_cast<uintptr_t>(pVirtualAddress) & 0x3FFFFFFF));
		return RETVAL_OK;
	}
	

	volatile PML2_t *_PML2 = reinterpret_cast<volatile PML2_t*>((reinterpret_cast<uintptr_t>(_PML3->Entry[_PML3Index].Address) << 12) + 
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML2=0x");
	CConsole::Print(htoa((uint64_t)_PML2, _TempText));
	CConsole::Print("\n");	 */
	if (_PML2->Entry[_PML2Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	
	if (_PML2->Entry[_PML2Index].PageSize) {
		// CConsole::Print("CPaging::GetInitialPhysicalAddress() - 2M page size\n");
		*pPhysicalAddress = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_PML2->Entry2M[_PML2Index].Address) << 12) &
			(reinterpret_cast<uintptr_t>(pVirtualAddress) & 0xFFFFF));
		return RETVAL_OK;
	}
	
	volatile PML1_t *_PML1 = reinterpret_cast<volatile PML1_t*>((reinterpret_cast<uintptr_t>(_PML2->Entry[_PML2Index].Address) << 12) +
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML1=0x");
	CConsole::Print(htoa((uint64_t)_PML1, _TempText));
	CConsole::Print("\n"); */
	if (_PML1->Entry[_PML1Index].Present) {
		return RETVAL_OK;
	}
	
	// CConsole::Print("CPaging::GetInitialPhysicalAddress() - 4K page size\n");
	*pPhysicalAddress = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_PML1->Entry[_PML1Index].Address) << 12) +
		(reinterpret_cast<uintptr_t>(pVirtualAddress) & 0xFFF));
	return RETVAL_OK;
}


static ReturnValue_t MapInitialAddress(void *pVirtualAddress, void *pPhysicalAddress, PageLevel_t pPageLevel) {
	
	if ((pPageLevel != PAGELEVEL_PML1) && (pPageLevel != PAGELEVEL_PML2))
		return RETVAL_ERROR_INVALID_PAGELEVEL;
	// Renable once support for 1G pages
/* 	if (pPageLevel != PAGELEVEL_PML3)
		return RETVAL_ERROR_INVALID_PAGELEVEL; */
	
	
	char _TempText[24];
	uint32_t _PML5Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 48) & 0x1FF;
/* 	CConsole::Print("MapInitialAddress() - _PML5Index=");
	CConsole::Print(itoa(_PML5Index, _TempText, 10));
	CConsole::Print("\n"); */
	uint32_t _PML4Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 39) & 0x1FF;
/* 	CConsole::Print("MapInitialAddress() - _PML4Index=");
	CConsole::Print(itoa(_PML4Index, _TempText, 10));
	CConsole::Print("\n"); */
	uint32_t _PML3Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 30) & 0x1FF;
/* 	CConsole::Print("MapInitialAddress() - _PML3Index=");
	CConsole::Print(itoa(_PML3Index, _TempText, 10));
	CConsole::Print("\n"); */
	uint32_t _PML2Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 21) & 0x1FF;
/* 	CConsole::Print("MapInitialAddress() - _PML2Index=");
	CConsole::Print(itoa(_PML2Index, _TempText, 10));
	CConsole::Print("\n"); */
	uint32_t _PML1Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 12) & 0x1FF;
/* 	CConsole::Print("MapInitialAddress() - _PML1Index=");
	CConsole::Print(itoa(_PML1Index, _TempText, 10));
	CConsole::Print("\n"); */
	
	
	
	volatile PML4_t *_PML4 = reinterpret_cast<volatile PML4_t*>(reinterpret_cast<uintptr_t>(CPaging::GetCR3()) + CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("MapInitialAddress() - _PML4=0x");
	CConsole::Print(htoa((uint64_t)_PML4, _TempText));
	CConsole::Print("\n"); */
	if (_PML4->Entry[_PML4Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	
	
	volatile PML3_t *_PML3 = reinterpret_cast<volatile PML3_t*>((reinterpret_cast<uintptr_t>(_PML4->Entry[_PML4Index].Address) << 12) + 
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("MapInitialAddress() - _PML3=0x");
	CConsole::Print(htoa((uint64_t)_PML3, _TempText));
	CConsole::Print("\n");	 */
	
	if (_PML3->Entry[_PML3Index].PageSize) {
		if (pPageLevel != PAGELEVEL_PML3)
			return RETVAL_ERROR_INVALID_PAGELEVEL;
		
		_PML3->Entry1G[_PML3Index].Address = reinterpret_cast<uint64_t>(reinterpret_cast<uintptr_t>(pPhysicalAddress) >> 30);
		_PML3->Entry1G[_PML3Index].Present = 1;
		return RETVAL_OK;
	}


	if (_PML3->Entry[_PML3Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	volatile PML2_t *_PML2 = reinterpret_cast<volatile PML2_t*>((reinterpret_cast<uintptr_t>(_PML3->Entry[_PML3Index].Address) << 12) + 
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("MapInitialAddress() - _PML2=0x");
	CConsole::Print(htoa((uint64_t)_PML2, _TempText));
	CConsole::Print("\n");	 */
	
	if (_PML2->Entry[_PML2Index].PageSize) {
		if (pPageLevel != PAGELEVEL_PML2)
			return RETVAL_ERROR_INVALID_PAGELEVEL;
		
		_PML2->Entry2M[_PML2Index].Address = reinterpret_cast<uint64_t>(reinterpret_cast<uintptr_t>(pPhysicalAddress) >> 21);
		_PML2->Entry2M[_PML2Index].Present = 1;
		return RETVAL_OK;
	}
	

	if (_PML2->Entry[_PML2Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	volatile PML1_t *_PML1 = reinterpret_cast<volatile PML1_t*>((reinterpret_cast<uintptr_t>(_PML2->Entry[_PML2Index].Address) << 12) + 
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("MapInitialAddress() - _PML1=0x");
	CConsole::Print(htoa((uint64_t)_PML1, _TempText));
	CConsole::Print("\n");	 */
	
	if (pPageLevel != PAGELEVEL_PML1)
		return RETVAL_ERROR_INVALID_PAGELEVEL;
	
	_PML1->Entry[_PML1Index].Address = reinterpret_cast<uint64_t>(reinterpret_cast<uintptr_t>(pPhysicalAddress) >> 12);
	_PML1->Entry[_PML1Index].Present = 1;
	return RETVAL_OK;
}

static PageLevel_t GetInitialPageLevel(void *pVirtualAddress) {
	
	char _TempText[24];
	
	uint32_t _PML5Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 48) & 0x1FF;
/* 	CConsole::Print("GetInitialPageLevel() - _PML5Index=");
	CConsole::Print(itoa(_PML5Index, _TempText, 10));
	CConsole::Print("\n"); */
	uint32_t _PML4Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 39) & 0x1FF;
/* 	CConsole::Print("GetInitialPageLevel() - _PML4Index=");
	CConsole::Print(itoa(_PML4Index, _TempText, 10));
	CConsole::Print("\n"); */
	uint32_t _PML3Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 30) & 0x1FF;
/* 	CConsole::Print("GetInitialPageLevel() - _PML3Index=");
	CConsole::Print(itoa(_PML3Index, _TempText, 10));
	CConsole::Print("\n"); */
	uint32_t _PML2Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 21) & 0x1FF;
/* 	CConsole::Print("GetInitialPageLevel() - _PML2Index=");
	CConsole::Print(itoa(_PML2Index, _TempText, 10));
	CConsole::Print("\n"); */
	uint32_t _PML1Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 12) & 0x1FF;
/* 	CConsole::Print("GetInitialPageLevel() - _PML1Index=");
	CConsole::Print(itoa(_PML1Index, _TempText, 10));
	CConsole::Print("\n"); */
	
	
	
	volatile PML4_t *_PML4 = reinterpret_cast<volatile PML4_t*>(reinterpret_cast<uintptr_t>(CPaging::GetCR3()) + CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPageLevel() - _PML4=0x");
	CConsole::Print(htoa((uint64_t)_PML4, _TempText));
	CConsole::Print("\n"); */
	if (_PML4->Entry[_PML4Index].Present == 0) {
		return PAGELEVEL_UNKNOWN;
	}
	
	
	volatile PML3_t *_PML3 = reinterpret_cast<volatile PML3_t*>((reinterpret_cast<uintptr_t>(_PML4->Entry[_PML4Index].Address) << 12) + 
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPageLevel() - _PML3=0x");
	CConsole::Print(htoa((uint64_t)_PML3, _TempText));
	CConsole::Print("\n");	 */
	
	if (_PML3->Entry[_PML3Index].PageSize) {
		if (_PML3->Entry1G[_PML3Index].Present == 0)
			return PAGELEVEL_UNKNOWN;
		return PAGELEVEL_PML3;
	}
	
	if (_PML3->Entry[_PML3Index].Present == 0) {
		return PAGELEVEL_UNKNOWN;
	}
	volatile PML2_t *_PML2 = reinterpret_cast<volatile PML2_t*>((reinterpret_cast<uintptr_t>(_PML3->Entry[_PML3Index].Address) << 12) + 
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPageLevel() - _PML2=0x");
	CConsole::Print(htoa((uint64_t)_PML2, _TempText));
	CConsole::Print("\n");	 */
	
	if (_PML2->Entry[_PML2Index].PageSize) {
		if (_PML2->Entry2M[_PML2Index].Present == 0)
			return PAGELEVEL_UNKNOWN;
		return PAGELEVEL_PML2;
	}	
	
	
	if (_PML2->Entry[_PML2Index].Present == 0) {
		return PAGELEVEL_UNKNOWN;
	}
	volatile PML1_t *_PML1 = reinterpret_cast<volatile PML1_t*>((reinterpret_cast<uintptr_t>(_PML2->Entry[_PML2Index].Address) << 12) + 
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPageLevel() - _PML1=0x");
	CConsole::Print(htoa((uint64_t)_PML1, _TempText));
	CConsole::Print("\n");	 */
	
	if (_PML1->Entry[_PML1Index].Present)
		return PAGELEVEL_PML1;
	
	return PAGELEVEL_UNKNOWN;
}


void *CPaging::GetCR3(void) {
	
	void *_CR3 = NULL;
	asm volatile (
		"movq %%cr3, %%rax;\n"
		"movq %%rax, %0;\n"
		: "=m" (_CR3)
		: 
		: "rax"
	);
	
	return _CR3;
}

const char *CPaging::GetPageLevelString(void *pVirtualAddress) {
	return GetPageLevelString(GetPageLevel(pVirtualAddress));
}

const char *CPaging::GetPageLevelString(PageLevel_t pPageLevel) {
	switch (pPageLevel) {
		case PAGELEVEL_PML1:
			return "PML1";
		case PAGELEVEL_PML2:
			return "PML2";
		case PAGELEVEL_PML3:
			return "PML3";
		case PAGELEVEL_PML4:
			return "PML4";
		case PAGELEVEL_PML5:
			return "PML5";
		default:
			break;
	}
	return "UNKNOWN";
}

ReturnValue_t CPaging::GetPhysicalAddress(void *pVirtualAddress, void **pPhysicalAddress) {
	
	if (IsInitial) {
		return GetInitialPhysicalAddress(pVirtualAddress, pPhysicalAddress);
	}
	
	*pPhysicalAddress = NULL;
	return RETVAL_ERROR_PAGE_NOT_FOUND;
}

ReturnValue_t CPaging::MapAddress(void *pVirtualAddress, void *pPhysicalAddress, PageLevel_t pPageLevel) {

	if (IsInitial) {
		return MapInitialAddress(pVirtualAddress, pPhysicalAddress, pPageLevel);
	}

	return RETVAL_ERROR_GENERAL;
}

PageLevel_t CPaging::GetPageLevel(void *pVirtualAddress) {
	
	if (IsInitial) {
		return GetInitialPageLevel(pVirtualAddress);
	}

	return PAGELEVEL_UNKNOWN;	
}

