
#ifndef MULTIBOOT2_HEADER
#define MULTIBOOT2_HEADER

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MULTIBOOT2_INFO_MAGIC					0x36D76289
#define MULTIBOOT2_INFO_ALIGN					0x00000008

#define MULTIBOOT2_TAG_TYPE_ALIGN				8
#define MULTIBOOT2_TAG_TYPE_END					0
#define MULTIBOOT2_TAG_TYPE_CMDLINE				1
#define MULTIBOOT2_TAG_TYPE_BOOT_LOADER_NAME	2
#define MULTIBOOT2_TAG_TYPE_MODULE				3
#define MULTIBOOT2_TAG_TYPE_BASIC_MEMINFO		4
#define MULTIBOOT2_TAG_TYPE_BOOTDEV				5
#define MULTIBOOT2_TAG_TYPE_MMAP				6
#define MULTIBOOT2_TAG_TYPE_VBE					7
#define MULTIBOOT2_TAG_TYPE_FRAMEBUFFER			8
#define MULTIBOOT2_TAG_TYPE_ELF_SECTIONS		9
#define MULTIBOOT2_TAG_TYPE_APM					10
#define MULTIBOOT2_TAG_TYPE_EFI32				11
#define MULTIBOOT2_TAG_TYPE_EFI64				12
#define MULTIBOOT2_TAG_TYPE_SMBIOS				13
#define MULTIBOOT2_TAG_TYPE_ACPI_OLD			14
#define MULTIBOOT2_TAG_TYPE_ACPI_NEW			15
#define MULTIBOOT2_TAG_TYPE_NETWORK				16
#define MULTIBOOT2_TAG_TYPE_EFI_MMAP			17
#define MULTIBOOT2_TAG_TYPE_EFI_BS				18
#define MULTIBOOT2_TAG_TYPE_EFI32_IH			19
#define MULTIBOOT2_TAG_TYPE_EFI64_IH			20
#define MULTIBOOT2_TAG_TYPE_LOAD_BASE_ADDR		21

#define MULTIBOOT2_MEMORY_AVAILABLE				1
#define MULTIBOOT2_MEMORY_RESERVED				2
#define MULTIBOOT2_MEMORY_ACPI_RECLAIMABLE		3
#define MULTIBOOT2_MEMORY_NVS					4
#define MULTIBOOT2_MEMORY_BADRAM				5


struct multiboot2_info_t {
	uint32_t total_size;
	uint32_t reserved;
} __attribute__((packed));

struct multiboot2_info_tag_t {
	uint32_t type;
	uint32_t size;
};

struct multiboot2_info_tag_cmdline_t {
	uint32_t type;
	uint32_t size;
	char string[0];
};

struct multiboot2_info_tag_boot_loader_name_t {
	uint32_t type;
	uint32_t size;
	char string[0];
};

struct multiboot2_info_tag_module_t {
	uint32_t type;
	uint32_t size;
	uint32_t mod_start;
	uint32_t mod_end;
	char cmdline[0];
};

struct multiboot2_info_tag_basic_meminfo_t {
	uint32_t type;
	uint32_t size;
	uint32_t mem_lower;
	uint32_t mem_upper;
};


#endif
