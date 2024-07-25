
#ifndef HEADER_PAGING
#define HEADER_PAGING

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "retvals.hpp"

struct PML4Entry_t {
	uint64_t present:1;
	uint64_t write:1;
	uint64_t user:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t available:1;
	uint64_t reserved:2;
	uint64_t available_l:3;
	uint64_t base:40;
	uint64_t available_h:11;
	uint64_t execute_disable:1;
} __attribute__((packed));

struct PML3Entry_t {
	uint64_t present:1;
	uint64_t write:1;
	uint64_t user:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t available:1;
	uint64_t reserved:1;
	uint64_t global:1;
	uint64_t available_l:3;
	uint64_t base:40;
	uint64_t available_h:11;
	uint64_t execute_disable:1;
} __attribute__((packed));

struct PML3Entry1GB_t {
	uint64_t present:1;
	uint64_t write:1;
	uint64_t user:1;
	uint64_t write_through:1;
	uint64_t cache_disable:1;
	uint64_t accessed:1;
	uint64_t available:1;
	uint64_t size:1;
	uint64_t global:1;
	uint64_t available_l:3;
	uint64_t page_attribute_table:1;
	uint64_t reserved2:17;
	uint64_t base:22;
	uint64_t available_h:11;
	uint64_t execute_disable:1;
} __attribute__((packed));

struct PML4Table_t {
	PML4Entry_t entry[512];
} __attribute__((packed, aligned (4096) ));

struct PML3Table_t {
	union {
		PML3Entry_t entry[512];
		PML3Entry1GB_t entry1GB[512];
	};
} __attribute__((packed, aligned (4096) ));


class Paging {
private:
	
	static PML4Table_t PML4T;
	static PML3Table_t PML3THigh;
	static PML3Table_t PML3TLow;

	Paging();
	~Paging();
public:
	
	static retval_t Init(void);
	static void* GetPML4T(void);
	
};

#endif
