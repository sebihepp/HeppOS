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

#ifndef HEADER_PAGING
#define HEADER_PAGING

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <retval.h>

enum PageLevel_t {
	PAGELEVEL_UNKNOWN = 0,
	PAGELEVEL_PML1 = 1,
	PAGELEVEL_PML2 = 2,
	PAGELEVEL_PML3 = 3,
	PAGELEVEL_PML4 = 4,
	PAGELEVEL_PML5 = 5,
};

enum CachType_t {
	CACHETYPE_WRITEBACK = 0,
	CACHETYPE_WRITETHROUGH = 1,
	CACHETYPE_UNCACHED = 2,
	CACHETYPE_UNCACHABLE = 3,
	CACHETYPE_WRITEPROTECT = 4,
	CACHETYPE_WRITECOMBINING = 5,
};

enum PagingCapability_t {
	PAGINGCAPABILITY_1GPAGES = 0,
	PAGINGCAPABILITY_PML5,
	PAGINGCAPABILITY_COUNT,
};

//////

struct PML5Entry_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Dirty:1;
	uint64_t PageSize:1;
	uint64_t Global:1;
	uint64_t Available1:2;
	uint64_t Reserved1:1;
	uint64_t Address:40;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML5Entry_256T_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Dirty:1;
	uint64_t PageSize:1;
	uint64_t Global:1;
	uint64_t Available1:2;
	uint64_t Reserved1:1;
	uint64_t PAT:1;
	uint64_t Reserved2:35;
	uint64_t Address:4;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML5_t {
	union {
		PML5Entry_t Entry[512];
		PML5Entry_256T_t Entry256T[512];
	} __attribute__(( packed ));
} __attribute__ (( packed, aligned(1024) ));

//////

struct PML4Entry_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Dirty:1;
	uint64_t PageSize:1;
	uint64_t Global:1;
	uint64_t Available1:2;
	uint64_t Reserved1:1;
	uint64_t Address:40;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML4Entry_512G_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Dirty:1;
	uint64_t PageSize:1;
	uint64_t Global:1;
	uint64_t Available1:2;
	uint64_t Reserved1:1;
	uint64_t PAT:1;
	uint64_t Reserved2:26;
	uint64_t Address:13;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML4_t {
	union {
		PML4Entry_t Entry[512];
		PML4Entry_512G_t Entry512G[512];
	} __attribute__(( packed ));
} __attribute__ (( packed, aligned(1024) ));

//////

struct PML3Entry_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Dirty:1;
	uint64_t PageSize:1;
	uint64_t Global:1;
	uint64_t Available1:2;
	uint64_t Reserved1:1;
	uint64_t Address:40;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML3Entry_1G_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Dirty:1;
	uint64_t PageSize:1;
	uint64_t Global:1;
	uint64_t Available1:2;
	uint64_t Reserved1:1;
	uint64_t PAT:1;
	uint64_t Reserved2:17;
	uint64_t Address:22;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML3_t {
	union {
		PML3Entry_t Entry[512];
		PML3Entry_1G_t Entry1G[512];
	} __attribute__(( packed ));
} __attribute__ (( packed, aligned(1024) ));

//////

struct PML2Entry_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Dirty:1;
	uint64_t PageSize:1;
	uint64_t Global:1;
	uint64_t Available1:2;
	uint64_t Reserved1:1;
	uint64_t Address:40;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML2Entry_2M_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Dirty:1;
	uint64_t PageSize:1;
	uint64_t Global:1;
	uint64_t Available1:2;
	uint64_t Reserved1:1;
	uint64_t PAT:1;
	uint64_t Reserved2:8;
	uint64_t Address:31;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML2_t {
	union {
		PML2Entry_t Entry[512];
		PML2Entry_2M_t Entry2M[512];
	} __attribute__(( packed ));
} __attribute__ (( packed, aligned(1024) ));

//////

struct PML1Entry_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Dirty:1;
	uint64_t PAT:1;
	uint64_t Global:1;
	uint64_t Available1:2;
	uint64_t Reserved1:1;
	uint64_t Address:40;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML1_t {
	PML1Entry_t Entry[512];
} __attribute__ (( packed, aligned(1024) ));

class CPaging {
private:
	CPaging();
	~CPaging();


	static bool mCapabilities[PAGINGCAPABILITY_COUNT];
	static bool mActiveCapabilities[PAGINGCAPABILITY_COUNT];
	
	static bool mIsInitial;	
	static void *mHHDMOffset;
	
public:

	static ReturnValue_t PreInit(void) __attribute__ (( nothrow ));
	
	static inline void *GetCR3(void) __attribute__ (( nothrow )) {
		void *_CR3 = NULL;
		asm volatile (
			"movq %%cr3, %0;\n"
			: "=a" (_CR3)
			: 
			:
		);
		return _CR3;
	}
	static inline void InvalidateAddress(void *pAddress) __attribute__ (( nothrow )) {
		asm volatile (
			"invlpg (%0);\n"
			:
			: "r" (pAddress)
			: "memory"
		);	
	}
	
	static inline void *GetHHDMOffset(void) __attribute__ (( nothrow )) {
		return mHHDMOffset;
	}
	
	static ReturnValue_t GetPhysicalAddress(void *pVirtualAddress, void *&pPhysicalAddress) __attribute__ (( nothrow ));
	static ReturnValue_t GetPageLevel(void *pVirtualAddress, PageLevel_t &pPageLevel) __attribute__ (( nothrow ));
	
	static ReturnValue_t MapAddress(void *pVirtualAddress, void *pPhysicalAddress, PageLevel_t pPageLevel, CachType_t pCacheType, 
		bool pGlobal, bool pExecuteDisable, bool pUser) __attribute__ (( nothrow ));
	static ReturnValue_t UnmapAddress(void *pVirtualAddress, PageLevel_t pPageLevel) __attribute__ (( nothrow ));
	
	static const char *GetPageLevelString(PageLevel_t pPageLevel) __attribute__ (( const, nothrow ));
	static const char *GetPageLevelString(void *pVirtualAddress) __attribute__ (( nothrow ));
	
	static inline bool GetCapability(PagingCapability_t pCapability) __attribute__ (( nothrow )) {
		return mCapabilities[pCapability];
	}
	
	static bool GetActiveCapability(PagingCapability_t pCapability) __attribute__ (( nothrow )) {
		return mActiveCapabilities[pCapability];
	}

};


#endif
