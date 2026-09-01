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
 
#include <memory/paging.h>
#include <limine_stub.h>
#include <boot_log.h>
#include <cpu/cpuid.h>
#include <kstring.h>


bool Paging::is_initial_ = true;
VirtualAddress Paging::hhdm_offset_ = (VirtualAddress)NULL;
bool Paging::capabilities_[PagingCapabilityCount];
bool Paging::active_capabilities_[PagingCapabilityCount];
	
/* void *Paging::GetCR3(void) {
	
	void *_CR3 = NULL;
	asm volatile (
		"movq %%cr3, %0;\n"
		: "=a" (_CR3)
		: 
		:
	);
	
	return _CR3;
} */

/* void Paging::InvalidateAddress(void *address) {
	
	asm volatile (
		"invlpg %0;\n"
		:
		: "m" (address)
		:
	);
	
} */

const char *Paging::GetPageLevelString(VirtualAddress virtual_address) {
	PageLevel page_level = PageLevelUnknown;
	GetPageLevel(virtual_address, page_level);
	return GetPageLevelString(page_level);
}

const char *Paging::GetPageLevelString(PageLevel page_level) {
	switch (page_level) {
		case PageLevelPML1:
			return "PML1";
		case PageLevelPML2:
			return "PML2";
		case PageLevelPML3:
			return "PML3";
		case PageLevelPML4:
			return "PML4";
		case PageLevelPML5:
			return "PML5";
		case PageLevelUnknown:
			return "UNKNOWN";
		default:
			break;
	}
	return "UNKNOWN";
}

ReturnValue Paging::GetPhysicalAddress(VirtualAddress virtual_address,
	 PhysicalAddress &physical_address) {
	
	uint32_t pml5_index = (virtual_address >> 48) & 0x1FF;
	uint32_t pml4_index = (virtual_address >> 39) & 0x1FF;
	uint32_t pml3_index = (virtual_address >> 30) & 0x1FF;
	uint32_t pml2_index = (virtual_address >> 21) & 0x1FF;
	uint32_t pml1_index = (virtual_address >> 12) & 0x1FF;
	
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::GetPhysicalAddress() - pml5_index=%d\n", 
		pml5_index);
 	BootLog::PrintF("CPaging::GetPhysicalAddress() - pml4_index=%d\n", 
		pml4_index);
 	BootLog::PrintF("CPaging::GetPhysicalAddress() - pml3_index=%d\n", 
		pml3_index);
 	BootLog::PrintF("CPaging::GetPhysicalAddress() - pml2_index=%d\n", 
		pml2_index);
 	BootLog::PrintF("CPaging::GetPhysicalAddress() - pml1_index=%d\n", 
		pml1_index);
#endif
	
	
	volatile PML4 *pml4 = NULL;
	
	if (Paging::GetActiveCapability(PagingCapabilityPML5)) {

		volatile PML5 *pml5 = reinterpret_cast<volatile PML5*>(
			Paging::GetCR3() + Paging::GetHHDMOffset());

#ifdef _DEBUG
		BootLog::PrintF("CPaging::GetPhysicalAddress() - pml5=%p\n", pml5);
#endif
		if (pml5->entry[pml5_index].present == 0) {
			return ReturnValueErrorPageNotPresent;
		}
		
		if (pml5->entry[pml5_index].page_size) {
#ifdef _DEBUG
			BootLog::Print("CPaging::GetPhysicalAddress() - 256T page size\n");
#endif
			physical_address = ((PhysicalAddress)
				pml5->entry_256t[pml5_index].address << 48) & 
				(virtual_address & 0xFFFFFFFFFFFF);
			return ReturnValueOk;
		}
		
		pml4 = reinterpret_cast<volatile PML4*>(((PhysicalAddress)
			pml5->entry[pml5_index].address << 12) + Paging::GetHHDMOffset());
		
	} else {
		pml4 = reinterpret_cast<volatile PML4*>(Paging::GetCR3() + 
			Paging::GetHHDMOffset());
	}
	
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::GetPhysicalAddress() - pml4=%p\n", pml4);
#endif
	if (pml4->entry[pml4_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	
	if (Paging::GetActiveCapability(PagingCapabilityPML5) && 
		pml4->entry[pml4_index].page_size) {
#ifdef _DEBUG
		BootLog::Print("CPaging::GetPhysicalAddress() - 512G page size\n");
#endif
		physical_address = ((PhysicalAddress)
			pml4->entry_512g[pml4_index].address << 39) &
			(virtual_address & 0x7FFFFFFFFF);
		return ReturnValueOk;
	}
	
	
	volatile PML3 *pml3 = reinterpret_cast<volatile PML3*>
		(((PhysicalAddress)pml4->entry[pml4_index].address << 12) + 
		Paging::GetHHDMOffset());
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::GetPhysicalAddress() - pml3=%p\n", pml3);
#endif
	if (pml3->entry[pml3_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}

	if (pml3->entry[pml3_index].page_size) {
#ifdef _DEBUG
		BootLog::Print("CPaging::GetPhysicalAddress() - 1G page size\n");
#endif
		physical_address = ((PhysicalAddress)
			pml3->entry_1g[pml3_index].address << 30) & 
			(virtual_address & 0x3FFFFFFF);
		return ReturnValueOk;
	}
	

	volatile PML2 *pml2 = reinterpret_cast<volatile PML2*>(
		((PhysicalAddress)pml3->entry[pml3_index].address << 12) + 
		Paging::GetHHDMOffset());
#ifdef _DEBUG
	BootLog::PrintF("CPaging::GetPhysicalAddress() - pml2=%p\n", pml2);
#endif
	if (pml2->entry[pml2_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	
	if (pml2->entry[pml2_index].page_size) {
#ifdef _DEBUG
		BootLog::Print("CPaging::GetPhysicalAddress() - 2M page size\n");
#endif
		physical_address = ((PhysicalAddress)
			pml2->entry_2m[pml2_index].address << 21) & (virtual_address & 0x1FFFFF);
		return ReturnValueOk;
	}
	
	volatile PML1 *pml1 = reinterpret_cast<volatile PML1*>(
		((PhysicalAddress)pml2->entry[pml2_index].address << 12) +
		Paging::GetHHDMOffset());
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::GetPhysicalAddress() - pml1=%p\n", pml1);
#endif
	if (pml1->entry[pml1_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	
#ifdef _DEBUG
	BootLog::Print("CPaging::GetPhysicalAddress() - 4K page size\n");
#endif
	physical_address = ((PhysicalAddress)
	pml1->entry[pml1_index].address << 12) + (virtual_address & 0xFFF);
	return ReturnValueOk;
	
}

ReturnValue Paging::MapAddress(VirtualAddress virtual_address, PhysicalAddress physical_address, PageLevel page_level,
		CacheType cache_type, bool global, bool execute_disable, bool user) {

	if (page_level == PageLevelUnknown) {
		return ReturnValueErrorInvalidPageLevel;
	}
	if (Paging::GetActiveCapability(PagingCapabilityPML5) == false) {
		if ((page_level == PageLevelPML4) || (page_level == PageLevelPML5)) {
			return ReturnValueErrorInvalidPageLevel;
		}
	}
	if ((page_level == PageLevelPML3) && (Paging::GetActiveCapability(PagingCapability1GPages) == false)) {
		return ReturnValueErrorInvalidPageLevel;
	}

	uint32_t pml5_index = (virtual_address >> 48) & 0x1FF;
	uint32_t pml4_index = (virtual_address >> 39) & 0x1FF;
	uint32_t pml3_index = (virtual_address >> 30) & 0x1FF;
	uint32_t pml2_index = (virtual_address >> 21) & 0x1FF;
	uint32_t pml1_index = (virtual_address >> 12) & 0x1FF;
	
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::MapAddress() - pml5_index=%d\n", pml5_index);
 	BootLog::PrintF("CPaging::MapAddress() - pml4_index=%d\n", pml4_index);
 	BootLog::PrintF("CPaging::MapAddress() - pml3_index=%d\n", pml3_index);
 	BootLog::PrintF("CPaging::MapAddress() - pml2_index=%d\n", pml2_index);
 	BootLog::PrintF("CPaging::MapAddress() - pml1_index=%d\n", pml1_index);
#endif	
	
	volatile PML4 *pml4 = NULL;
	
	if (Paging::GetActiveCapability(PagingCapabilityPML5)) {

		volatile PML5 *pml5 = reinterpret_cast<volatile PML5*>(Paging::GetCR3() + Paging::GetHHDMOffset());
#ifdef _DEBUG
		BootLog::PrintF("CPaging::MapAddress() - pml5=%p\n", pml5); 
#endif
		
		if (pml5->entry[pml5_index].present == 0) {
			return ReturnValueErrorPageNotPresent;
		}
		
		if (pml5->entry[pml5_index].page_size) {
			if (page_level != PageLevelPML5)
				return ReturnValueErrorInvalidPageLevel;
			
			pml5->entry_256t[pml5_index].address = physical_address >> 48;
			pml5->entry_256t[pml5_index].present = 1;
			pml5->entry_256t[pml5_index].global = (global) ? 1 : 0;
			pml5->entry_256t[pml5_index].execute_disable = (execute_disable) ? 1 : 0;
			pml5->entry_256t[pml5_index].not_supervisor = (user) ? 1 : 0;
			pml5->entry_256t[pml5_index].write_through = (cache_type & 0x1) ? 1 : 0;
			pml5->entry_256t[pml5_index].cache_disable = (cache_type & 0x2) ? 1 : 0;
			pml5->entry_256t[pml5_index].pat = (cache_type & 0x4) ? 1 : 0;
			InvalidateAddress(virtual_address);
			return ReturnValueOk;
		}
		
		pml4 = reinterpret_cast<volatile PML4*>(((PhysicalAddress)pml5->entry[pml5_index].address << 12) + Paging::GetHHDMOffset());
	} else {
		pml4 = reinterpret_cast<volatile PML4*>(Paging::GetCR3() + Paging::GetHHDMOffset());
	}
	
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::MapAddress() - pml4=%p\n", pml4);
#endif
	if (pml4->entry[pml4_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	if (Paging::GetActiveCapability(PagingCapabilityPML5) && pml4->entry[pml4_index].page_size) {
		if (page_level != PageLevelPML4)
			return ReturnValueErrorInvalidPageLevel;
		
		pml4->entry_512g[pml4_index].address = physical_address >> 39;
		pml4->entry_512g[pml4_index].present = 1;
		pml4->entry_512g[pml4_index].global = (global) ? 1 : 0;
		pml4->entry_512g[pml4_index].execute_disable = (execute_disable) ? 1 : 0;
		pml4->entry_512g[pml4_index].not_supervisor = (user) ? 1 : 0;
		pml4->entry_512g[pml4_index].write_through = (cache_type & 0x1) ? 1 : 0;
		pml4->entry_512g[pml4_index].cache_disable = (cache_type & 0x2) ? 1 : 0;
		pml4->entry_512g[pml4_index].pat = (cache_type & 0x4) ? 1 : 0;
		InvalidateAddress(virtual_address);
		return ReturnValueOk;
	}
	
	volatile PML3 *pml3 = reinterpret_cast<volatile PML3*>(((PhysicalAddress)pml4->entry[pml4_index].address << 12) + 
		Paging::GetHHDMOffset());
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::MapAddress() - pml3=%p\n", pml3);
#endif
	if (pml3->entry[pml3_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	
	if (pml3->entry[pml3_index].page_size) {
		if (page_level != PageLevelPML3)
			return ReturnValueErrorInvalidPageLevel;
		
		pml3->entry_1g[pml3_index].address = physical_address >> 30;
		pml3->entry_1g[pml3_index].present = 1;
		pml3->entry_1g[pml3_index].global = (global) ? 1 : 0;
		pml3->entry_1g[pml3_index].execute_disable = (execute_disable) ? 1 : 0;
		pml3->entry_1g[pml3_index].not_supervisor = (user) ? 1 : 0;
		pml3->entry_1g[pml3_index].write_through = (cache_type & 0x1) ? 1 : 0;
		pml3->entry_1g[pml3_index].cache_disable = (cache_type & 0x2) ? 1 : 0;
		pml3->entry_1g[pml3_index].pat = (cache_type & 0x4) ? 1 : 0;
		InvalidateAddress(virtual_address);
		return ReturnValueOk;
	}

	volatile PML2 *pml2 = reinterpret_cast<volatile PML2*>(((PhysicalAddress)pml3->entry[pml3_index].address << 12) + 
		Paging::GetHHDMOffset());
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::MapAddress() - pml2=%p\n", pml2);
#endif
	
	if (pml2->entry[pml2_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	if (pml2->entry[pml2_index].page_size) {
		if (page_level != PageLevelPML2)
			return ReturnValueErrorInvalidPageLevel;
		
		pml2->entry_2m[pml2_index].address = physical_address >> 21;
		pml2->entry_2m[pml2_index].present = 1;
		pml2->entry_2m[pml2_index].global = (global) ? 1 : 0;
		pml2->entry_2m[pml2_index].execute_disable = (execute_disable) ? 1 : 0;
		pml2->entry_2m[pml2_index].not_supervisor = (user) ? 1 : 0;
		pml2->entry_2m[pml2_index].write_through = (cache_type & 0x1) ? 1 : 0;
		pml2->entry_2m[pml2_index].cache_disable = (cache_type & 0x2) ? 1 : 0;
		pml2->entry_2m[pml2_index].pat = (cache_type & 0x4) ? 1 : 0;
		InvalidateAddress(virtual_address);
		return ReturnValueOk;
	}

	volatile PML1 *pml1 = reinterpret_cast<volatile PML1*>(
		((PhysicalAddress)pml2->entry[pml2_index].address << 12
	) + Paging::GetHHDMOffset());
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::MapAddress() - pml1=%p\n", pml1);
#endif
	if (pml1->entry[pml1_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	if (page_level != PageLevelPML1) {
		return ReturnValueErrorInvalidPageLevel;
	}
	
	pml1->entry[pml1_index].address = physical_address >> 12;
	pml1->entry[pml1_index].present = 1;
	pml1->entry[pml1_index].global = (global) ? 1 : 0;
	pml1->entry[pml1_index].execute_disable = (execute_disable) ? 1 : 0;
	pml1->entry[pml1_index].not_supervisor = (user) ? 1 : 0;
	pml1->entry[pml1_index].write_through = (cache_type & 0x1) ? 1 : 0;
	pml1->entry[pml1_index].cache_disable = (cache_type & 0x2) ? 1 : 0;
	pml1->entry[pml1_index].pat = (cache_type & 0x4) ? 1 : 0;
	InvalidateAddress(virtual_address);
	
	return ReturnValueOk;
	
}

ReturnValue Paging::UnmapAddress(VirtualAddress virtual_address, PageLevel page_level) {

	if (page_level == PageLevelUnknown) {
		return ReturnValueErrorInvalidPageLevel;
	}
	if (Paging::GetActiveCapability(PagingCapabilityPML5) == false) {
		if ((page_level == PageLevelPML4) || (page_level == PageLevelPML5)) {
			return ReturnValueErrorInvalidPageLevel;
		}
	}
	if ((page_level == PageLevelPML3) && 
		(Paging::GetActiveCapability(PagingCapability1GPages) == false)) {
		return ReturnValueErrorInvalidPageLevel;
	}

	uint32_t pml5_index = (virtual_address >> 48) & 0x1FF;
	uint32_t pml4_index = (virtual_address >> 39) & 0x1FF;
	uint32_t pml3_index = (virtual_address >> 30) & 0x1FF;
	uint32_t pml2_index = (virtual_address >> 21) & 0x1FF;
	uint32_t pml1_index = (virtual_address >> 12) & 0x1FF;
	
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::UnmapAddress() - _PML5Index=%d\n", pml5_index);
 	BootLog::PrintF("CPaging::UnmapAddress() - _PML4Index=%d\n", pml4_index);
 	BootLog::PrintF("CPaging::UnmapAddress() - _PML3Index=%d\n", pml3_index);
 	BootLog::PrintF("CPaging::UnmapAddress() - _PML2Index=%d\n", pml2_index);
 	BootLog::PrintF("CPaging::UnmapAddress() - _PML1Index=%d\n", pml1_index);
#endif	
	
	volatile PML4 *pml4 = NULL;
	
	if (Paging::GetActiveCapability(PagingCapabilityPML5)) {

		volatile PML5 *pml5 = reinterpret_cast<volatile PML5*>(Paging::GetCR3() + Paging::GetHHDMOffset());
#ifdef _DEBUG
		BootLog::PrintF("CPaging::UnmapAddress() - _PML5=%p\n", pml5);
#endif
		if (pml5->entry[pml5_index].present == 0) {
			return ReturnValueErrorPageNotPresent;
		}
		
		if (pml5->entry[pml5_index].page_size) {
			if (page_level != PageLevelPML5)
				return ReturnValueErrorInvalidPageLevel;
			
			pml5->entry_256t[pml5_index].present = 0;
			InvalidateAddress(virtual_address);
			return ReturnValueOk;
		}
		
		pml4 = reinterpret_cast<volatile PML4*>(((PhysicalAddress)pml5->entry[pml5_index].address << 12) +
			Paging::GetHHDMOffset());
	} else {
		pml4 = reinterpret_cast<volatile PML4*>(Paging::GetCR3() + Paging::GetHHDMOffset());
	}
	
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::UnmapAddress() - _PML4=%p\n", pml4);
#endif
	if (pml4->entry[pml4_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	if (pml4->entry[pml4_index].page_size) {
		if (page_level != PageLevelPML4)
			return ReturnValueErrorInvalidPageLevel;
		
		pml4->entry_512g[pml4_index].present = 0;
		InvalidateAddress(virtual_address);
		return ReturnValueOk;
	}
	
	volatile PML3 *pml3 = reinterpret_cast<volatile PML3*>(((PhysicalAddress)pml4->entry[pml4_index].address << 12) + 
		Paging::GetHHDMOffset());
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::UnmapAddress() - _PML3=%p\n", pml3);
#endif
	if (pml3->entry[pml3_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	
	if (pml3->entry[pml3_index].page_size) {
		if (page_level != PageLevelPML3)
			return ReturnValueErrorInvalidPageLevel;
		
		pml3->entry_1g[pml3_index].present = 0;
		InvalidateAddress(virtual_address);
		return ReturnValueOk;
	}

	volatile PML2 *pml2 = reinterpret_cast<volatile PML2*>(((PhysicalAddress)pml3->entry[pml3_index].address << 12) + 
		Paging::GetHHDMOffset());
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::UnmapAddress() - _PML2=%p\n", pml2);
#endif
	
	if (pml2->entry[pml2_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	if (pml2->entry[pml2_index].page_size) {
		if (page_level != PageLevelPML2)
			return ReturnValueErrorInvalidPageLevel;
		
		pml2->entry_2m[pml2_index].present = 0;
		InvalidateAddress(virtual_address);
		return ReturnValueOk;
	}


	volatile PML1 *pml1 = reinterpret_cast<volatile PML1*>(((PhysicalAddress)pml2->entry[pml2_index].address << 12) + 
		Paging::GetHHDMOffset());
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::UnmapAddress() - _PML1=%p\n", pml1);
#endif
	if (pml1->entry[pml1_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	if (page_level != PageLevelPML1)
		return ReturnValueErrorInvalidPageLevel;
	
	pml1->entry[pml1_index].present = 0;
	InvalidateAddress(virtual_address);

	return ReturnValueOk;
}

ReturnValue Paging::GetPageLevel(VirtualAddress virtual_address, PageLevel &page_level) {

	uint32_t pml5_index = (virtual_address >> 48) & 0x1FF;
	uint32_t pml4_index = (virtual_address >> 39) & 0x1FF;
	uint32_t pml3_index = (virtual_address >> 30) & 0x1FF;
	uint32_t pml2_index = (virtual_address >> 21) & 0x1FF;
	uint32_t pml1_index = (virtual_address >> 12) & 0x1FF;
	
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::GetPageLevel() - _PML5Index=%d\n", pml5_index);
 	BootLog::PrintF("CPaging::GetPageLevel() - _PML4Index=%d\n", pml4_index);
 	BootLog::PrintF("CPaging::GetPageLevel() - _PML3Index=%d\n", pml3_index);
 	BootLog::PrintF("CPaging::GetPageLevel() - _PML2Index=%d\n", pml2_index);
 	BootLog::PrintF("CPaging::GetPageLevel() - _PML1Index=%d\n", pml1_index);
#endif
	
	volatile PML4 *pml4 = NULL;
	
	if (Paging::GetActiveCapability(PagingCapabilityPML5)) {

		volatile PML5 *pml5 = reinterpret_cast<volatile PML5*>(Paging::GetCR3() + Paging::GetHHDMOffset());
#ifdef _DEBUG
		BootLog::PrintF("CPaging::GetPageLevel() - _PML5=%p\n", pml5);
#endif

		page_level = PageLevelPML5;
		if (pml5->entry[pml5_index].present == 0) {
			return ReturnValueErrorPageNotPresent;
		}
		
		if (pml5->entry[pml5_index].page_size) {
			return ReturnValueOk;
		}
		
		pml4 = reinterpret_cast<volatile PML4*>(((PhysicalAddress)pml5->entry[pml5_index].address << 12) +
			Paging::GetHHDMOffset());
	} else {
		pml4 = reinterpret_cast<volatile PML4*>(Paging::GetCR3() + Paging::GetHHDMOffset());
	}
	
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::GetPageLevel() - _PML4=%p\n", pml4);
#endif
	
	page_level = PageLevelPML4;
	if (pml4->entry[pml4_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	
	if (pml4->entry[pml4_index].page_size) {
		return ReturnValueOk;
	}	
	
	volatile PML3 *pml3 = reinterpret_cast<volatile PML3*>(((PhysicalAddress)pml4->entry[pml4_index].address << 12) + 
		Paging::GetHHDMOffset());
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::GetPageLevel() - _PML3=%p\n", pml3);
#endif
	
	page_level = PageLevelPML3;
	if (pml3->entry[pml3_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	if (pml3->entry[pml3_index].page_size) {
		return ReturnValueOk;
	}

	volatile PML2 *pml2 = reinterpret_cast<volatile PML2*>(((PhysicalAddress)pml3->entry[pml3_index].address << 12) + 
		Paging::GetHHDMOffset());
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::GetPageLevel() - _PML2=%p\n", pml2);
#endif
	page_level = PageLevelPML2;
	
	if (pml2->entry[pml2_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	
	if (pml2->entry[pml2_index].page_size) {
		return ReturnValueOk;
	}
		
	volatile PML1 *pml1 = reinterpret_cast<volatile PML1*>(((PhysicalAddress)pml2->entry[pml2_index].address << 12) + 
		Paging::GetHHDMOffset());
#ifdef _DEBUG
 	BootLog::PrintF("CPaging::GetPageLevel() - _PML1=%p\n", pml1);
#endif
	page_level = PageLevelPML1;
	if (pml1->entry[pml1_index].present == 0) {
		return ReturnValueErrorPageNotPresent;
	}
	return ReturnValueOk;
}

ReturnValue Paging::PreInit(void) {

	CPUIdRetVal cpu_ret_val;
	uint64_t cr4;
	
	for (uint32_t i = 0; i < PagingCapabilityCount; ++i) {
		capabilities_[i] = false;
		active_capabilities_[i] = false;
	}
	
	// Check for PML5 support and if it is active
	cpuid(0x00000000, cpu_ret_val);
	if (cpu_ret_val.eax >= 0x00000007) {
		cpuid(0x80000007, cpu_ret_val);
		if (cpu_ret_val.ecx & (1 << 16)) {
			
			capabilities_[PagingCapabilityPML5] = true;
			
			// Check if PML5 is active
			asm volatile (
				"mov %%cr4, %%rax;\n"
				"mov %%rax, %0;\n"
				: "=m" (cr4)
				:
				: "rax"
			);
			if (cr4 & (1 << 12)) {
				active_capabilities_[PagingCapabilityPML5] = true;
			}			
		}
	}	
	
	// Check for 1G-pages support
	cpuid(0x80000000, cpu_ret_val);
	if (cpu_ret_val.eax >= 0x80000001) {
		cpuid(0x80000001, cpu_ret_val);
		if (cpu_ret_val.edx & (1 << 26)) {
			capabilities_[PagingCapability1GPages] = true;
			active_capabilities_[PagingCapability1GPages] = true;
		}
	}
	
	// Save limine HHDM
	hhdm_offset_ = (DeltaAddress)LimineStub::GetHHDMResponse()->offset;
	
	return ReturnValueOk;
}
