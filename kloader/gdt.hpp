
#ifndef GDT_HEADER
#define GDT_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "retvals.hpp"

#define GDT_NULL 		0
#define GDT_CODE32 		1
#define GDT_DATA32 		2
#define GDT_CODE64 		3
#define GDT_DATA64 		4
#define GDT_CODE16 		5
#define GDT_DATA16 		6
#define GDT_COUNT		7

#define GDT_NULL_SEL		0x00
#define GDT_CODE32_SEL		0x08
#define GDT_DATA32_SEL		0x10
#define GDT_CODE64_SEL		0x18
#define GDT_DATA64_SEL		0x20
#define GDT_CODE16_SEL		0x28
#define GDT_DATA16_SEL		0x30


#define GDT_ACCESS_PRESENT			0x80
#define GDT_ACCESS_DPL0				0x00
#define GDT_ACCESS_DPL1				0x20
#define GDT_ACCESS_DPL2				0x40
#define GDT_ACCESS_DPL3				0x60
#define GDT_ACCESS_DATA_CODE		0x10
#define GDT_ACCESS_SYSTEM_SEGMENT	0x00
#define GDT_ACCESS_EXECUTE			0x08
#define GDT_ACCESS_DIRECTION_DOWN	0x04
#define GDT_ACCESS_DIRECTION_UP		0x00
#define GDT_ACCESS_CONFORM			0x04
#define GDT_ACCESS_RW				0x02
#define GDT_ACCESS_ACCESS			0x01

#define GDT_FLAGS_GRANULARITY		0x8
#define GDT_FLAGS_32BIT				0x4
#define GDT_FLAGS_16BIT				0x0
#define GDT_FLAGS_64BIT				0x2

#define GDT_ENTRY_SIZE		0x8

struct GDTD_t {
	uint16_t size;
	uint32_t offset;
}__attribute__((packed));

struct GD_t {
	uint16_t limit_l;
	uint16_t base_l;
	uint8_t base_m;
	uint8_t access;
	uint8_t limit_h:4;
	uint8_t flags:4;
	uint8_t base_h;
}__attribute__((packed));


class GDT {
private:
	GDT();
	~GDT();
	
	static GDTD_t GlobalDescriptorTableDescriptor;
	static GD_t GlobalDescriptorTable[GDT_COUNT];
	
public:	
	static retval_t Init(void);
	
	static void LoadGDT(void);
	
};

#endif
