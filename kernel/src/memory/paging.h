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

#pragma once

#include <ktype.h>

#define PAGE_SIZE (4096)

enum PageLevel {
	PageLevelUnknown = 0,
	PageLevelPML1 = 1,
	PageLevelPML2 = 2,
	PageLevelPML3 = 3,
	PageLevelPML4 = 4,
	PageLevelPML5 = 5,
};

enum CacheType {
	CacheTypeWriteBack = 0,
	CacheTypeWriteThrough = 1,
	CacheTypeUncached = 2,
	CacheTypeUncachable = 3,
	CacheTypeWriteProtect = 4,
	CacheTypeWriteCombining = 5,
};

enum PagingCapability {
	PagingCapability1GPages = 0,
	PagingCapabilityPML5,
	PagingCapabilityCount,
};

//////

struct PML5Entry {
	uint64_t present:1;
	uint64_t read_write:1;
	uint64_t not_supervisor:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t dirty:1;
	uint64_t page_size:1;
	uint64_t global:1;
	uint64_t available1:2;
	uint64_t reserved1:1;
	uint64_t address:40;
	uint64_t available2:11;
	uint64_t execute_disable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML5Entry_256T {
	uint64_t present:1;
	uint64_t read_write:1;
	uint64_t not_supervisor:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t dirty:1;
	uint64_t page_size:1;
	uint64_t global:1;
	uint64_t available1:2;
	uint64_t reserved1:1;
	uint64_t pat:1;
	uint64_t reserved2:35;
	uint64_t address:4;
	uint64_t available2:11;
	uint64_t execute_disable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML5 {
	union {
		PML5Entry entry[512];
		PML5Entry_256T entry_256t[512];
	} __attribute__(( packed ));
} __attribute__ (( packed, aligned(1024) ));

//////

struct PML4Entry {
	uint64_t present:1;
	uint64_t read_write:1;
	uint64_t not_supervisor:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t dirty:1;
	uint64_t page_size:1;
	uint64_t global:1;
	uint64_t available1:2;
	uint64_t reserved1:1;
	uint64_t address:40;
	uint64_t available2:11;
	uint64_t execute_disable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML4Entry_512G {
	uint64_t present:1;
	uint64_t read_write:1;
	uint64_t not_supervisor:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t dirty:1;
	uint64_t page_size:1;
	uint64_t global:1;
	uint64_t available1:2;
	uint64_t reserved1:1;
	uint64_t pat:1;
	uint64_t reserved2:26;
	uint64_t address:13;
	uint64_t available2:11;
	uint64_t execute_disable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML4 {
	union {
		PML4Entry entry[512];
		PML4Entry_512G entry_512g[512];
	} __attribute__(( packed ));
} __attribute__ (( packed, aligned(1024) ));

//////

struct PML3Entry {
	uint64_t present:1;
	uint64_t read_write:1;
	uint64_t not_supervisor:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t dirty:1;
	uint64_t page_size:1;
	uint64_t global:1;
	uint64_t available1:2;
	uint64_t reserved1:1;
	uint64_t address:40;
	uint64_t available2:11;
	uint64_t execute_disable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML3Entry_1G {
	uint64_t present:1;
	uint64_t read_write:1;
	uint64_t not_supervisor:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t dirty:1;
	uint64_t page_size:1;
	uint64_t global:1;
	uint64_t available1:2;
	uint64_t reserved1:1;
	uint64_t pat:1;
	uint64_t reserved2:17;
	uint64_t address:22;
	uint64_t available2:11;
	uint64_t execute_disable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML3 {
	union {
		PML3Entry entry[512];
		PML3Entry_1G entry_1g[512];
	} __attribute__(( packed ));
} __attribute__ (( packed, aligned(1024) ));

//////

struct PML2Entry {
	uint64_t present:1;
	uint64_t read_write:1;
	uint64_t not_supervisor:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t dirty:1;
	uint64_t page_size:1;
	uint64_t global:1;
	uint64_t available1:2;
	uint64_t reserved1:1;
	uint64_t address:40;
	uint64_t available2:11;
	uint64_t execute_disable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML2Entry_2M {
	uint64_t present:1;
	uint64_t read_write:1;
	uint64_t not_supervisor:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t dirty:1;
	uint64_t page_size:1;
	uint64_t global:1;
	uint64_t available1:2;
	uint64_t reserved1:1;
	uint64_t pat:1;
	uint64_t reserved2:8;
	uint64_t address:31;
	uint64_t available2:11;
	uint64_t execute_disable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML2 {
	union {
		PML2Entry entry[512];
		PML2Entry_2M entry_2m[512];
	} __attribute__(( packed ));
} __attribute__ (( packed, aligned(1024) ));

//////

struct PML1Entry {
	uint64_t present:1;
	uint64_t read_write:1;
	uint64_t not_supervisor:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t dirty:1;
	uint64_t pat:1;
	uint64_t global:1;
	uint64_t available1:2;
	uint64_t reserved1:1;
	uint64_t address:40;
	uint64_t available2:11;
	uint64_t execute_disable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML1 {
	PML1Entry entry[512];
} __attribute__ (( packed, aligned(1024) ));

class Paging {
private:
	Paging();
	~Paging();


	static bool capabilities_[PagingCapabilityCount];
	static bool active_capabilities_[PagingCapabilityCount];
	
	static bool is_initial_;	
	static VirtualAddress hhdm_offset_;
	
public:

	static ReturnValue PreInit(void) __attribute__((nothrow));
	
	static inline PhysicalAddress GetCR3(void)
	    __attribute__((nothrow, always_inline)) {
		PhysicalAddress cr3 = (PhysicalAddress)NULL;
		asm volatile (
			"movq %%cr3, %0;\n"
			: "=a" (cr3)
			:
			:
		);
		return cr3;
	}
	static inline void InvalidateAddress(VirtualAddress virtual_address)
	    __attribute__((nothrow, always_inline)) {
		asm volatile (
			"invlpg (%0);\n"
			:
			: "r" (virtual_address)
			: "memory"
		);	
	}
	
	static inline DeltaAddress GetHHDMOffset(void)
	    __attribute__((nothrow, always_inline)) {
		return hhdm_offset_;
	}
	
	static ReturnValue GetPhysicalAddress(VirtualAddress virtual_address,
	    PhysicalAddress &physical_address) __attribute__((nothrow));
	static ReturnValue GetPageLevel(VirtualAddress virtual_address,
	    PageLevel &page_level) __attribute__((nothrow));
	
	static ReturnValue MapAddress(VirtualAddress virtual_address,
	    PhysicalAddress physical_address, PageLevel page_level,
	    CacheType cache_type, bool global, bool execute_disable,
	    bool user) __attribute__((nothrow));
	static ReturnValue UnmapAddress(VirtualAddress virtual_address,
	    PageLevel page_level) __attribute__((nothrow));
	
	static const char *GetPageLevelString(PageLevel page_level)
	    __attribute__((const, nothrow));
	static const char *GetPageLevelString(VirtualAddress virtual_address)
	    __attribute__((nothrow));
	
	static inline bool GetCapability(PagingCapability capability)
	    __attribute__((nothrow, always_inline)) {
		return capabilities_[capability];
	}
	
	static bool GetActiveCapability(PagingCapability capability)
	    __attribute__((nothrow, always_inline)) {
		return active_capabilities_[capability];
	}

};
