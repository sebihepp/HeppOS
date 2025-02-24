
#include <memory/paging.h>
#include <liminestub.h>
#include <video/console.h>
#include <cpu/cpuid.h>


// For quick testing - needs to be put in a string.h or something similar
char* itoa(int num, char* str, int base);
char* utoa(unsigned num, char* str, int base);
char *htoa(uint64_t num, char* str);

bool CPaging::mIsInitial = true;
bool CPaging::mUsesPML5 = false;
bool CPaging::mSupports1GPages = false;
void *CPaging::mHHDMAddress = NULL;

/* void *CPaging::GetCR3(void) {
	
	void *_CR3 = NULL;
	asm volatile (
		"movq %%cr3, %0;\n"
		: "=a" (_CR3)
		: 
		:
	);
	
	return _CR3;
} */

/* void CPaging::InvalidateAddress(void *pAddress) {
	
	asm volatile (
		"invlpg %0;\n"
		:
		: "m" (pAddress)
		:
	);
	
} */

const char *CPaging::GetPageLevelString(void *pVirtualAddress) {
	PageLevel_t _PageLevel = PAGELEVEL_UNKNOWN;
	GetPageLevel(pVirtualAddress, _PageLevel);
	return GetPageLevelString(_PageLevel);
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

ReturnValue_t CPaging::GetPhysicalAddress(void *pVirtualAddress, void *&pPhysicalAddress) {
	
#ifdef _DEBUG
	char _TempText[24];
#endif
	
	uint32_t _PML5Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 48) & 0x1FF;
	uint32_t _PML4Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 39) & 0x1FF;
	uint32_t _PML3Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 30) & 0x1FF;
	uint32_t _PML2Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 21) & 0x1FF;
	uint32_t _PML1Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 12) & 0x1FF;
	
#ifdef _DEBUG
 	CConsole::Print("CPaging::GetPhysicalAddress() - _PML5Index=");
	CConsole::Print(itoa(_PML5Index, _TempText, 10));
	CConsole::Print("\n");
 	CConsole::Print("CPaging::GetPhysicalAddress() - _PML4Index=");
	CConsole::Print(itoa(_PML4Index, _TempText, 10));
	CConsole::Print("\n");
	CConsole::Print("CPaging::GetPhysicalAddress() - _PML3Index=");
	CConsole::Print(itoa(_PML3Index, _TempText, 10));
	CConsole::Print("\n"); 
	CConsole::Print("CPaging::GetPhysicalAddress() - _PML2Index=");
	CConsole::Print(itoa(_PML2Index, _TempText, 10));
	CConsole::Print("\n");
 	CConsole::Print("CPaging::GetPhysicalAddress() - _PML1Index=");
	CConsole::Print(itoa(_PML1Index, _TempText, 10));
	CConsole::Print("\n");
#endif
	
	
	volatile PML4_t *_PML4 = NULL;
	
	if (CPaging::GetUsesPLM5()) {

		volatile PML5_t *_PML5 = reinterpret_cast<volatile PML5_t*>(reinterpret_cast<uintptr_t>(CPaging::GetCR3()) + 
			reinterpret_cast<uintptr_t>(mHHDMAddress));

#ifdef _DEBUG
		CConsole::Print("CPaging::GetPhysicalAddress() - _PML5=0x");
		CConsole::Print(htoa((uint64_t)_PML5, _TempText));
		CConsole::Print("\n"); 
#endif
		if (_PML5->Entry[_PML5Index].Present == 0) {
			return RETVAL_ERROR_PAGE_NOT_PRESENT;
		}
		
		if (_PML5->Entry[_PML5Index].PageSize) {
#ifdef _DEBUG
			CConsole::Print("CPaging::GetPhysicalAddress() - 256T page size\n");
#endif
			pPhysicalAddress = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_PML5->Entry256T[_PML5Index].Address) << 48) &
				(reinterpret_cast<uintptr_t>(pVirtualAddress) & 0xFFFFFFFFFFFF));
			return RETVAL_OK;
		}
		
		_PML4 = reinterpret_cast<volatile PML4_t*>((reinterpret_cast<uintptr_t>(_PML5->Entry[_PML5Index].Address) << 12) +
			reinterpret_cast<uintptr_t>(mHHDMAddress));
		
	} else {
		_PML4 = reinterpret_cast<volatile PML4_t*>(reinterpret_cast<uintptr_t>(CPaging::GetCR3()) + 
			reinterpret_cast<uintptr_t>(mHHDMAddress));
	}
	
#ifdef _DEBUG
 	CConsole::Print("CPaging::GetPhysicalAddress() - _PML4=0x");
	CConsole::Print(htoa((uint64_t)_PML4, _TempText));
	CConsole::Print("\n");
#endif
	if (_PML4->Entry[_PML4Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	
	if (CPaging::GetUsesPLM5() && _PML4->Entry[_PML4Index].PageSize) {
#ifdef _DEBUG
		CConsole::Print("CPaging::GetPhysicalAddress() - 512G page size\n");
#endif
		pPhysicalAddress = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_PML4->Entry512G[_PML4Index].Address) << 39) &
			(reinterpret_cast<uintptr_t>(pVirtualAddress) & 0x7FFFFFFFFF));
		return RETVAL_OK;
	}
	
	
	volatile PML3_t *_PML3 = reinterpret_cast<volatile PML3_t*>((reinterpret_cast<uintptr_t>(_PML4->Entry[_PML4Index].Address) << 12) +
		reinterpret_cast<uintptr_t>(mHHDMAddress));
#ifdef _DEBUG
 	CConsole::Print("CPaging::GetPhysicalAddress() - _PML3=0x");
	CConsole::Print(htoa((uint64_t)_PML3, _TempText));
	CConsole::Print("\n");
#endif
	if (_PML3->Entry[_PML3Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}

	if (_PML3->Entry[_PML3Index].PageSize) {
#ifdef _DEBUG
		CConsole::Print("CPaging::GetPhysicalAddress() - 1G page size\n");
#endif
		pPhysicalAddress = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_PML3->Entry1G[_PML3Index].Address) << 30) &
			(reinterpret_cast<uintptr_t>(pVirtualAddress) & 0x3FFFFFFF));
		return RETVAL_OK;
	}
	

	volatile PML2_t *_PML2 = reinterpret_cast<volatile PML2_t*>((reinterpret_cast<uintptr_t>(_PML3->Entry[_PML3Index].Address) << 12) + 
		reinterpret_cast<uintptr_t>(mHHDMAddress));
#ifdef _DEBUG
	CConsole::Print("CPaging::GetPhysicalAddress() - _PML2=0x");
	CConsole::Print(htoa((uint64_t)_PML2, _TempText));
	CConsole::Print("\n");
#endif
	if (_PML2->Entry[_PML2Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	
	if (_PML2->Entry[_PML2Index].PageSize) {
#ifdef _DEBUG
		CConsole::Print("CPaging::GetPhysicalAddress() - 2M page size\n");
#endif
		pPhysicalAddress = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_PML2->Entry2M[_PML2Index].Address) << 21) &
			(reinterpret_cast<uintptr_t>(pVirtualAddress) & 0x1FFFFF));
		return RETVAL_OK;
	}
	
	volatile PML1_t *_PML1 = reinterpret_cast<volatile PML1_t*>((reinterpret_cast<uintptr_t>(_PML2->Entry[_PML2Index].Address) << 12) +
		reinterpret_cast<uintptr_t>(mHHDMAddress));
#ifdef _DEBUG
 	CConsole::Print("CPaging::GetPhysicalAddress() - _PML1=0x");
	CConsole::Print(htoa((uint64_t)_PML1, _TempText));
	CConsole::Print("\n"); 
#endif
	if (_PML1->Entry[_PML1Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	
#ifdef _DEBUG
	CConsole::Print("CPaging::GetPhysicalAddress() - 4K page size\n");
#endif
	pPhysicalAddress = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_PML1->Entry[_PML1Index].Address) << 12) +
		(reinterpret_cast<uintptr_t>(pVirtualAddress) & 0xFFF));
	return RETVAL_OK;
	
}

ReturnValue_t CPaging::MapAddress(void *pVirtualAddress, void *pPhysicalAddress, PageLevel_t pPageLevel) {
	
#ifdef _DEBUG
	char _TempText[24];
#endif

	if (pPageLevel == PAGELEVEL_UNKNOWN) {
		return RETVAL_ERROR_INVALID_PAGELEVEL;
	}
	if (CPaging::GetUsesPLM5() == false) {
		if ((pPageLevel == PAGELEVEL_PML4) || (pPageLevel == PAGELEVEL_PML5)) {
			return RETVAL_ERROR_INVALID_PAGELEVEL;
		}
	}
	if ((pPageLevel == PAGELEVEL_PML3) && (CPaging::GetSupports1GPages() == false)) {
		return RETVAL_ERROR_INVALID_PAGELEVEL;
	}

	uint32_t _PML5Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 48) & 0x1FF;
	uint32_t _PML4Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 39) & 0x1FF;
	uint32_t _PML3Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 30) & 0x1FF;
	uint32_t _PML2Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 21) & 0x1FF;
	uint32_t _PML1Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 12) & 0x1FF;
	
#ifdef _DEBUG
	CConsole::Print("CPaging::MapAddress() - _PML5Index=");
	CConsole::Print(itoa(_PML5Index, _TempText, 10));
	CConsole::Print("\n");
	CConsole::Print("CPaging::MapAddress() - _PML4Index=");
	CConsole::Print(itoa(_PML4Index, _TempText, 10));
	CConsole::Print("\n");
	CConsole::Print("CPaging::MapAddress() - _PML3Index=");
	CConsole::Print(itoa(_PML3Index, _TempText, 10));
	CConsole::Print("\n");
	CConsole::Print("CPaging::MapAddress() - _PML2Index=");
	CConsole::Print(itoa(_PML2Index, _TempText, 10));
	CConsole::Print("\n");
	CConsole::Print("CPaging::MapAddress() - _PML1Index=");
	CConsole::Print(itoa(_PML1Index, _TempText, 10));
	CConsole::Print("\n");
#endif	
	
	volatile PML4_t *_PML4 = NULL;
	
	if (CPaging::GetUsesPLM5()) {

		volatile PML5_t *_PML5 = reinterpret_cast<volatile PML5_t*>(reinterpret_cast<uintptr_t>(CPaging::GetCR3()) + 
			reinterpret_cast<uintptr_t>(mHHDMAddress));
#ifdef _DEBUG
		 CConsole::Print("CPaging::MapAddress() - _PML5=0x");
		CConsole::Print(htoa((uint64_t)_PML5, _TempText));
		CConsole::Print("\n"); 
#endif
		
		if (_PML5->Entry[_PML5Index].Present == 0) {
			return RETVAL_ERROR_PAGE_NOT_PRESENT;
		}
		
		if (_PML5->Entry[_PML5Index].PageSize) {
			if (pPageLevel != PAGELEVEL_PML5)
				return RETVAL_ERROR_INVALID_PAGELEVEL;
			
			_PML5->Entry256T[_PML5Index].Address = reinterpret_cast<uint64_t>(reinterpret_cast<uintptr_t>(pPhysicalAddress) >> 48);
			_PML5->Entry256T[_PML5Index].Present = 1;
			InvalidateAddress(pVirtualAddress);
			return RETVAL_OK;
		}
		
		_PML4 = reinterpret_cast<volatile PML4_t*>((reinterpret_cast<uintptr_t>(_PML5->Entry[_PML5Index].Address) << 12) +
			reinterpret_cast<uintptr_t>(mHHDMAddress));
	} else {
		_PML4 = reinterpret_cast<volatile PML4_t*>(reinterpret_cast<uintptr_t>(CPaging::GetCR3()) + 
			reinterpret_cast<uintptr_t>(mHHDMAddress));
	}
	
#ifdef _DEBUG
 	CConsole::Print("CPaging::MapAddress() - _PML4=0x");
	CConsole::Print(htoa((uint64_t)_PML4, _TempText));
	CConsole::Print("\n"); 
#endif
	if (_PML4->Entry[_PML4Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	if (_PML4->Entry[_PML4Index].PageSize) {
		if (pPageLevel != PAGELEVEL_PML4)
			return RETVAL_ERROR_INVALID_PAGELEVEL;
		
		_PML4->Entry512G[_PML3Index].Address = reinterpret_cast<uint64_t>(reinterpret_cast<uintptr_t>(pPhysicalAddress) >> 39);
		_PML4->Entry512G[_PML3Index].Present = 1;
		InvalidateAddress(pVirtualAddress);
		return RETVAL_OK;
	}
	
	volatile PML3_t *_PML3 = reinterpret_cast<volatile PML3_t*>((reinterpret_cast<uintptr_t>(_PML4->Entry[_PML4Index].Address) << 12) + 
		reinterpret_cast<uintptr_t>(mHHDMAddress));
#ifdef _DEBUG
 	CConsole::Print("CPaging::MapAddress() - _PML3=0x");
	CConsole::Print(htoa((uint64_t)_PML3, _TempText));
	CConsole::Print("\n");
#endif
	if (_PML3->Entry[_PML3Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	
	if (_PML3->Entry[_PML3Index].PageSize) {
		if (pPageLevel != PAGELEVEL_PML3)
			return RETVAL_ERROR_INVALID_PAGELEVEL;
		
		_PML3->Entry1G[_PML3Index].Address = reinterpret_cast<uint64_t>(reinterpret_cast<uintptr_t>(pPhysicalAddress) >> 30);
		_PML3->Entry1G[_PML3Index].Present = 1;
		InvalidateAddress(pVirtualAddress);
		return RETVAL_OK;
	}

	volatile PML2_t *_PML2 = reinterpret_cast<volatile PML2_t*>((reinterpret_cast<uintptr_t>(_PML3->Entry[_PML3Index].Address) << 12) + 
		reinterpret_cast<uintptr_t>(mHHDMAddress));
#ifdef _DEBUG
 	CConsole::Print("CPaging::MapAddress() - _PML2=0x");
	CConsole::Print(htoa((uint64_t)_PML2, _TempText));
	CConsole::Print("\n");
#endif
	
	if (_PML2->Entry[_PML2Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	if (_PML2->Entry[_PML2Index].PageSize) {
		if (pPageLevel != PAGELEVEL_PML2)
			return RETVAL_ERROR_INVALID_PAGELEVEL;
		
		_PML2->Entry2M[_PML2Index].Address = reinterpret_cast<uint64_t>(reinterpret_cast<uintptr_t>(pPhysicalAddress) >> 21);
		_PML2->Entry2M[_PML2Index].Present = 1;
		InvalidateAddress(pVirtualAddress);
		return RETVAL_OK;
	}


	volatile PML1_t *_PML1 = reinterpret_cast<volatile PML1_t*>((reinterpret_cast<uintptr_t>(_PML2->Entry[_PML2Index].Address) << 12) + 
		reinterpret_cast<uintptr_t>(mHHDMAddress));
#ifdef _DEBUG
 	CConsole::Print("CPaging::MapAddress() - _PML1=0x");
	CConsole::Print(htoa((uint64_t)_PML1, _TempText));
	CConsole::Print("\n");
#endif
	if (_PML1->Entry[_PML1Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	if (pPageLevel != PAGELEVEL_PML1)
		return RETVAL_ERROR_INVALID_PAGELEVEL;
	
	_PML1->Entry[_PML1Index].Address = reinterpret_cast<uint64_t>(reinterpret_cast<uintptr_t>(pPhysicalAddress) >> 12);
	_PML1->Entry[_PML1Index].Present = 1;
	InvalidateAddress(pVirtualAddress);
	
	return RETVAL_OK;
	
}

ReturnValue_t CPaging::GetPageLevel(void *pVirtualAddress, PageLevel_t &pPageLevel) {
	
#ifdef _DEBUG	
	char _TempText[24];
#endif	

	uint32_t _PML5Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 48) & 0x1FF;
	uint32_t _PML4Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 39) & 0x1FF;
	uint32_t _PML3Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 30) & 0x1FF;
	uint32_t _PML2Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 21) & 0x1FF;
	uint32_t _PML1Index = (reinterpret_cast<uintptr_t>(pVirtualAddress) >> 12) & 0x1FF;
	
#ifdef _DEBUG
	CConsole::Print("CPaging::GetPageLevel() - _PML5Index=");
	CConsole::Print(itoa(_PML5Index, _TempText, 10));
	CConsole::Print("\n");
	CConsole::Print("CPaging::GetPageLevel() - _PML4Index=");
	CConsole::Print(itoa(_PML4Index, _TempText, 10));
	CConsole::Print("\n");
	CConsole::Print("CPaging::GetPageLevel() - _PML3Index=");
	CConsole::Print(itoa(_PML3Index, _TempText, 10));
	CConsole::Print("\n");
	CConsole::Print("CPaging::GetPageLevel() - _PML2Index=");
	CConsole::Print(itoa(_PML2Index, _TempText, 10));
	CConsole::Print("\n");
	CConsole::Print("CPaging::GetPageLevel() - _PML1Index=");
	CConsole::Print(itoa(_PML1Index, _TempText, 10));
	CConsole::Print("\n");
#endif
	
	volatile PML4_t *_PML4 = NULL;
	
	if (CPaging::GetUsesPLM5()) {

		volatile PML5_t *_PML5 = reinterpret_cast<volatile PML5_t*>(reinterpret_cast<uintptr_t>(CPaging::GetCR3()) +
			reinterpret_cast<uintptr_t>(mHHDMAddress));
#ifdef _DEBUG
		CConsole::Print("CPaging::GetPageLevel() - _PML5=0x");
		CConsole::Print(htoa((uint64_t)_PML5, _TempText));
		CConsole::Print("\n");
#endif

		pPageLevel = PAGELEVEL_PML5;
		if (_PML5->Entry[_PML5Index].Present == 0) {
			return RETVAL_ERROR_PAGE_NOT_PRESENT;
		}
		
		if (_PML5->Entry[_PML5Index].PageSize) {
			return RETVAL_OK;
		}
		
		_PML4 = reinterpret_cast<volatile PML4_t*>((reinterpret_cast<uintptr_t>(_PML5->Entry[_PML5Index].Address) << 12) +
			reinterpret_cast<uintptr_t>(mHHDMAddress));
	} else {
		_PML4 = reinterpret_cast<volatile PML4_t*>(reinterpret_cast<uintptr_t>(CPaging::GetCR3()) + 
			reinterpret_cast<uintptr_t>(mHHDMAddress));
	}
	
#ifdef _DEBUG
 	CConsole::Print("CPaging::GetPageLevel() - _PML4=0x");
	CConsole::Print(htoa((uint64_t)_PML4, _TempText));
	CConsole::Print("\n"); 
#endif
	
	pPageLevel = PAGELEVEL_PML4;
	if (_PML4->Entry[_PML4Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	
	if (_PML4->Entry[_PML4Index].PageSize) {
		return RETVAL_OK;
	}	
	
	volatile PML3_t *_PML3 = reinterpret_cast<volatile PML3_t*>((reinterpret_cast<uintptr_t>(_PML4->Entry[_PML4Index].Address) << 12) + 
		reinterpret_cast<uintptr_t>(mHHDMAddress));
#ifdef _DEBUG
 	CConsole::Print("CPaging::GetPageLevel() - _PML3=0x");
	CConsole::Print(htoa((uint64_t)_PML3, _TempText));
	CConsole::Print("\n");
#endif
	
	pPageLevel = PAGELEVEL_PML3;
	if (_PML3->Entry[_PML3Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	if (_PML3->Entry[_PML3Index].PageSize) {
		return RETVAL_OK;
	}

	volatile PML2_t *_PML2 = reinterpret_cast<volatile PML2_t*>((reinterpret_cast<uintptr_t>(_PML3->Entry[_PML3Index].Address) << 12) + 
		reinterpret_cast<uintptr_t>(mHHDMAddress));
#ifdef _DEBUG
 	CConsole::Print("CPaging::GetPageLevel() - _PML2=0x");
	CConsole::Print(htoa((uint64_t)_PML2, _TempText));
	CConsole::Print("\n");
#endif
	pPageLevel = PAGELEVEL_PML2;
	
	if (_PML2->Entry[_PML2Index].Present == 0) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	
	if (_PML2->Entry[_PML2Index].PageSize) {
		return RETVAL_OK;
	}
		
	volatile PML1_t *_PML1 = reinterpret_cast<volatile PML1_t*>((reinterpret_cast<uintptr_t>(_PML2->Entry[_PML2Index].Address) << 12) + 
		reinterpret_cast<uintptr_t>(mHHDMAddress));
#ifdef _DEBUG
 	CConsole::Print("CPaging::GetPageLevel() - _PML1=0x");
	CConsole::Print(htoa((uint64_t)_PML1, _TempText));
	CConsole::Print("\n");
#endif
	pPageLevel = PAGELEVEL_PML1;
	if (_PML1->Entry[_PML1Index].Present)
		return RETVAL_OK;
	
	return RETVAL_ERROR_PAGE_NOT_PRESENT;
}

/* bool CPaging::GetUsesPLM5(void) {
	return mUsesPML5;
} */

/* bool CPaging::GetSupports1GPages(void) {
	return mSupports1GPages;
} */

ReturnValue_t CPaging::PreInit(void) {

	cpuid_retval_t _CPURetVal;
	uint64_t _CR4;
	
	// Check for PML5 support and if it is active
	cpuid(0x00000000, _CPURetVal);
	if (_CPURetVal.eax >= 0x00000007) {
		cpuid(0x80000007, _CPURetVal);
		if (_CPURetVal.ecx & (1 << 16)) {
			
			// Check if PML5 is active
			asm volatile (
				"mov %%cr4, %%rax;\n"
				"mov %%rax, %0;\n"
				: "=m" (_CR4)
				:
				: "rax"
			);
			if (_CR4 & (1 << 12)) {
				mUsesPML5 = true;
			} else {
				mUsesPML5 = false;
			}
			
		} else {
			mUsesPML5 = false;
		}
	} else {
		mUsesPML5 = false;
	}
		
	
	// Check for 1G-pages support
	cpuid(0x80000000, _CPURetVal);
	if (_CPURetVal.eax >= 0x80000001) {
		cpuid(0x80000001, _CPURetVal);
		if (_CPURetVal.edx & (1 << 26)) {
			mSupports1GPages = true;
		} else {
			mSupports1GPages = false;
		}
	} else {
		mSupports1GPages = false;
	}
	
	
	// Save limine HHDM
	mHHDMAddress = reinterpret_cast<void*>(CLimine::GetHHDMResponse()->offset);
	
	return RETVAL_OK;
}
