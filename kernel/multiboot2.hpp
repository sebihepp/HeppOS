
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
} __attribute__((packed));

struct multiboot2_info_tag_cmdline_t {
	uint32_t type;
	uint32_t size;
	
	char string[0];
} __attribute__((packed));

struct multiboot2_info_tag_boot_loader_name_t {
	uint32_t type;
	uint32_t size;
	
	char string[0];
} __attribute__((packed));

struct multiboot2_info_tag_module_t {
	uint32_t type;
	uint32_t size;
	
	uint32_t mod_start;
	uint32_t mod_end;
	char cmdline[0];
} __attribute__((packed));

struct multiboot2_info_tag_basic_meminfo_t {
	uint32_t type;
	uint32_t size;
	
	uint32_t mem_lower;
	uint32_t mem_upper;
} __attribute__((packed));

struct multiboot2_info_tag_bootdev_t {
	uint32_t type;
	uint32_t size;
	
	uint32_t biosdev;
	uint32_t partition;
	uint32_t subpartition;
} __attribute__((packed));

struct multiboot2_info_tag_mmap_t {
	uint32_t type;
	uint32_t size;
	
	uint32_t entry_size;
	uint32_t entry_version;
} __attribute__((packed));

struct multiboot2_info_tag_mmap_entry_t {
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
	uint32_t reserved;
} __attribute__((packed));

struct multiboot2_info_tag_vbe_t {
	uint32_t type;
	uint32_t size;
	
	uint16_t vbe_mode;
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_off;
	uint16_t vbe_interface_len;
	uint8_t vbe_control_info[512];
	uint8_t vbe_mode_info[256];
} __attribute__((packed));

struct multiboot2_info_tag_framebuffer_color_t {
	uint8_t red_value;
	uint8_t green_value;
	uint8_t blue_value;
} __attribute__((packed));

struct multiboot2_info_tag_framebuffer_palette_t {
	uint32_t num_colors;
	multiboot2_info_tag_framebuffer_color_t palette[0];
} __attribute__((packed));

struct multiboot2_info_tag_framebuffer_colortype_t {
	uint8_t red_field_position;
	uint8_t red_mask_size;
	uint8_t green_field_position;
	uint8_t green_mask_size;
	uint8_t blue_field_position;
	uint8_t blue_mask_size;
} __attribute__((packed));

union multiboot2_info_tag_framebuffer_colorinfo_t {
	multiboot2_info_tag_framebuffer_palette_t palette;
	multiboot2_info_tag_framebuffer_palette_t colortype;
} __attribute__((packed));
	
struct multiboot2_info_tag_framebuffer_t {
	uint32_t type;
	uint32_t size;
	
	uint64_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint8_t framebuffer_bpp;
	uint8_t framebuffer_type;
	uint8_t reserved;
	
	multiboot2_info_tag_framebuffer_colorinfo_t color_info;
} __attribute__((packed));

struct multiboot2_info_tag_elf_sections_t {
	uint32_t type;
	uint32_t size;
	
	uint16_t num;
	uint16_t entry_size;
	uint16_t shndx;
	uint16_t reserved;
	
	uint8_t sections[0];
	
} __attribute__((packed));

struct multiboot2_info_tag_apm_t {
	uint32_t type;
	uint32_t size;
	
	uint16_t version;
	uint16_t cseg;
	uint32_t offset;
	uint16_t cseg_16;
	uint16_t dseg;
	uint16_t flags;
	uint16_t cseg_length;
	uint16_t cseg_16_length;
	uint16_t dseg_length;	
} __attribute__((packed));

struct multiboot2_info_efi32_t {
	uint32_t type;
	uint32_t size;
	
	uint32_t pointer;
} __attribute__((packed));

struct multiboot2_info_efi64_t {
	uint32_t type;
	uint32_t size;
	
	uint64_t pointer;
} __attribute__((packed));

struct multiboot2_info_smbios_t {
	uint32_t type;
	uint32_t size;
	
	uint8_t major;
	uint8_t minor;
	uint8_t reserved[6];
	
	uint8_t smbios_tables[0];
} __attribute__((packed));

struct multiboot2_info_acpi_old_t {
	uint32_t type;
	uint32_t size;
	
	uint8_t RSDP[0];
} __attribute__((packed));

struct multiboot2_info_acpi_new_t {
	uint32_t type;
	uint32_t size;
	
	uint8_t RSDP[0];
} __attribute__((packed));

struct multiboot2_info_network_t {
	uint32_t type;
	uint32_t size;
	
	uint8_t dhcpack[0];
} __attribute__((packed));

struct multiboot2_info_efi_mmap_t {
	uint32_t type;
	uint32_t size;
	
	uint32_t descr_size;
	uint32_t descr_vers;
	uint8_t efi_mmap[0];
} __attribute__((packed));

struct multiboot2_info_efi32_ih_t {
	uint32_t type;
	uint32_t size;
	
	uint32_t pointer;
} __attribute__((packed));

struct multiboot2_info_efi64_ih_t {
	uint32_t type;
	uint32_t size;
	
	uint64_t pointer;
} __attribute__((packed));

struct multiboot2_info_load_base_addr_t {
	uint32_t type;
	uint32_t size;
	
	uint32_t load_base_addr;
} __attribute__((packed));


#endif
