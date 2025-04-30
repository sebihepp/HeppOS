
#ifndef GDT_HEADER
#define GDT_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <limine.h>

#include <retval.h>

#define GDT_ENTRY_SIZE				0x8
#define GDT_SYSTEM_ENTRY_SIZE		0x10

#define GDT_NULL 		0
#define GDT_CODE64 		1
#define GDT_DATA64 		2
#define GDT_TSS 		3

#define GDT_ENTRY_COUNT				0x03
#define GDT_TSS_ENTRY_COUNT			0x01
#define GDT_TOTAL_COUNT				(GDT_ENTRY_COUNT + (GDT_TSS_ENTRY_COUNT * 2))	//TSS is twice the size

#define GDT_NULL_SEL		0x00
#define GDT_CODE64_SEL		0x08
#define GDT_DATA64_SEL		0x10
#define GDT_TSS_SEL			0x18


#define GDT_ACCESS_PRESENT			0x80
#define GDT_ACCESS_DPL0				0x00
#define GDT_ACCESS_DPL1				0x20
#define GDT_ACCESS_DPL2				0x40
#define GDT_ACCESS_DPL3				0x60
#define GDT_ACCESS_DATA_CODE		0x10
#define GDT_ACCESS_TSS_SEGMENT		0x00
#define GDT_ACCESS_TSS_TYPE64		0x09
#define GDT_ACCESS_EXECUTE			0x08
#define GDT_ACCESS_DIRECTION_DOWN	0x04
#define GDT_ACCESS_DIRECTION_UP		0x00
#define GDT_ACCESS_CONFORM			0x04
#define GDT_ACCESS_RW				0x02
#define GDT_ACCESS_ACCESSED			0x01

#define GDT_FLAGS_GRANULARITY		0x8
#define GDT_FLAGS_32BIT				0x4
#define GDT_FLAGS_16BIT				0x0
#define GDT_FLAGS_64BIT				0x2



struct GDTD_t {
	uint16_t limit;
	uint64_t base;
}__attribute__(( packed, aligned (8) ));

struct GDTEntry_t {
	uint16_t limit_l;
	uint16_t base_l;
	uint8_t base_m;
	uint8_t access;
	uint8_t limit_h:4;
	uint8_t flags:4;
	uint8_t base_h;
}__attribute__(( packed, aligned (8) ));

struct GDTSystemEntry_t {
	uint16_t limit_l;
	uint16_t base_l;
	uint8_t base_m;
	uint8_t access;
	uint8_t limit_h:4;
	uint8_t flags:4;
	uint8_t base_h;
	uint32_t base_vh;
	uint32_t reserved;
}__attribute__(( packed, aligned (8) ));

struct TSS_t {
	uint32_t reserved0;
	uint64_t RSP0;
	uint64_t RSP1;
	uint64_t RSP2;
	uint64_t reserved1;
	uint64_t IST1;
	uint64_t IST2;
	uint64_t IST3;
	uint64_t IST4;
	uint64_t IST5;
	uint64_t IST6;
	uint64_t IST7;
	uint64_t reserved2;
	uint16_t reserved3;
	uint16_t IOPB_offset;
}__attribute__(( packed ));


class CGDT {
private:
	CGDT() = delete;
	~CGDT() = delete;
	
	static GDTD_t mGlobalDescriptorTableDescriptor;
	static GDTEntry_t mGlobalDescriptorTable[GDT_TOTAL_COUNT];
	static TSS_t mTaskStateSegment;
	
public:	
	static ReturnValue_t Init(void) __attribute__(( nothrow ));
	
	static void LoadGDT(void) __attribute__(( nothrow ));
	static void LoadTSS(void) __attribute__(( nothrow ));
	
	static TSS_t *GetTSS(void) __attribute__(( const, nothrow ));
	
	static uint16_t GetSelector(uint64_t pSelector) __attribute__(( const, nothrow ));
};

#endif

