
#ifndef HEADER_PAGING
#define HEADER_PAGING

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <retval.h>

enum PageLevel_t {
	PAGELEVEL_UNKNOWN = 0,
	PAGELEVEL_PML1,
	PAGELEVEL_PML2,
	PAGELEVEL_PML3,
	PAGELEVEL_PML4,
	PAGELEVEL_PML5,
};

struct PML4Entry_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Available1:1;
	uint64_t Zero1:1;
	uint64_t Available2:4;
	uint64_t Address:40;
	uint64_t Available3:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));

struct PML3Entry_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Available1:1;
	uint64_t PageSize:1;
	uint64_t Available2:4;
	uint64_t Address:40;
	uint64_t Available3:11;
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
	uint64_t Available1:3;
	uint64_t PAT:1;
	uint64_t Address:40;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));
struct PML2Entry_t {
	uint64_t Present:1;
	uint64_t ReadWrite:1;
	uint64_t NotSupervisor:1;
	uint64_t WriteThrough:1;
	uint64_t CacheDisable:1;
	uint64_t Accessed:1;
	uint64_t Ignored1:1;
	uint64_t PageSize:1;
	uint64_t Available1:4;
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
	uint64_t Available1:3;
	uint64_t PAT:1;
	uint64_t Address:40;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));

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
	uint64_t Available1:3;
	uint64_t Address:40;
	uint64_t Available2:11;
	uint64_t ExecuteDisable:1;
} __attribute__ (( packed, aligned(8) ));



class CPaging {
private:
	CPaging();
	~CPaging();

	static bool IsInitial;
	
public:

	static void *GetCR3(void);
	
	static ReturnValue_t GetPhysicalAddress(void *pVirtualAddress, void **pPhysicalAddress);
	static PageLevel_t GetPageLevel(void *pVirtualAddress);
	
	static ReturnValue_t MapAddress(void *pVirtualAddress, void *pPhysicalAddress, PageLevel_t pPageLevel);
	
	static const char *GetPageLevelString(PageLevel_t pPageLevel);
	static const char *GetPageLevelString(void *pVirtualAddress);
	

};

#endif
