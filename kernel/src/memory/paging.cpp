
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


	
	PML4Entry_t *_PML4 = reinterpret_cast<PML4Entry_t*>(reinterpret_cast<uintptr_t>(CPaging::GetCR3()) + CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML4=0x");
	CConsole::Print(htoa((uint64_t)_PML4, _TempText));
	CConsole::Print("\n"); */
	
	
	if (not _PML4[_PML4Index].Present) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	PML3Entry_t *_PML3 = reinterpret_cast<PML3Entry_t*>((reinterpret_cast<uintptr_t>(_PML4[_PML4Index].Address) << 12) +
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML3=0x");
	CConsole::Print(htoa((uint64_t)_PML3, _TempText));
	CConsole::Print("\n"); */
	if (_PML3[_PML3Index].PageSize) {
		// CConsole::Print("CPaging::GetInitialPhysicalAddress() - 1G page size\n");
		PML3Entry_1G_t *_PML3_1G = reinterpret_cast<PML3Entry_1G_t*>(_PML3);
		*pPhysicalAddress = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_PML3_1G[_PML3Index].Address) << 12) &
			(reinterpret_cast<uintptr_t>(pVirtualAddress) & 0x3FFFFFFF));
		if (_PML3_1G[_PML3Index].Present) {
			return RETVAL_OK;
		}
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	
	if (not _PML3[_PML3Index].Present) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	PML2Entry_t *_PML2 = reinterpret_cast<PML2Entry_t*>((reinterpret_cast<uintptr_t>(_PML3[_PML3Index].Address) << 12) + 
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML2=0x");
	CConsole::Print(htoa((uint64_t)_PML2, _TempText));
	CConsole::Print("\n");	 */
	if (_PML2[_PML2Index].PageSize) {
		// CConsole::Print("CPaging::GetInitialPhysicalAddress() - 2M page size\n");
		PML2Entry_2M_t *_PML2_2M = reinterpret_cast<PML2Entry_2M_t*>(_PML2);
		*pPhysicalAddress = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_PML2_2M[_PML2Index].Address) << 12) &
			(reinterpret_cast<uintptr_t>(pVirtualAddress) & 0xFFFFF));
		if (_PML2_2M[_PML2Index].Present) {
			return RETVAL_OK;
		}
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	
	if (not _PML2[_PML2Index].Present) {
		return RETVAL_ERROR_PAGE_NOT_PRESENT;
	}
	PML1Entry_t *_PML1 = reinterpret_cast<PML1Entry_t*>((reinterpret_cast<uintptr_t>(_PML2[_PML2Index].Address) << 12) +
		CLimine::GetHHDMResponse()->offset);
/* 	CConsole::Print("GetInitialPhysicalAddress() - _PML1=0x");
	CConsole::Print(htoa((uint64_t)_PML1, _TempText));
	CConsole::Print("\n"); */
	// CConsole::Print("CPaging::GetInitialPhysicalAddress() - 4K page size\n");
	*pPhysicalAddress = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_PML1[_PML1Index].Address) << 12) +
		(reinterpret_cast<uintptr_t>(pVirtualAddress) & 0xFFF));
	if (_PML1[_PML1Index].Present) {
		return RETVAL_OK;
	}
	return RETVAL_ERROR_PAGE_NOT_PRESENT;
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

ReturnValue_t CPaging::GetPhysicalAddress(void *pVirtualAddress, void **pPhysicalAddress) {
	
	if (IsInitial) {
		return GetInitialPhysicalAddress(pVirtualAddress, pPhysicalAddress);
	}
	
	*pPhysicalAddress = NULL;
	return RETVAL_ERROR_PAGE_NOT_FOUND;
}

