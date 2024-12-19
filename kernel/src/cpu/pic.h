
#ifndef PIC_HEADER
#define PIC_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <retvals.h>
#include <cpu/interrupt.h>


#define PIC_MASTER_COMMAND_PORT 	(0x20)
#define PIC_MASTER_DATA_PORT 		(0x21)

#define PIC_SLAVE_COMMAND_PORT		(0xA0)
#define PIC_SLAVE_DATA_PORT 		(0xA1)

#define PIC_EOI			(0x20)
#define PIC_READ_IRR	(0x0A)
#define PIC_READ_ISR	(0x0B)

#define PIC_ICW1_ICW4		(0x01)
#define PIC_ICW1_SINGLE		(0x02)
#define PIC_ICW1_INTERVAL4	(0x04)
#define PIC_ICW1_LEVEL		(0x08)
#define PIC_ICW1_INIT		(0x10)

#define PIC_ICW4_8086				(0x01)
#define PIC_ICW4_AUTO				(0x02)
#define PIC_ICW4_BUFFERED_SLAVE		(0x08)
#define PIC_ICW4_BUFFERED_MASTER	(0x0C)
#define PIC_ICW4_SFNM				(0x10)


class CPIC {
private:
	CPIC();
	~CPIC();
	
	static uint8_t mOffset;
	static uint64_t mSpuriousCount;
	
public:
	static ReturnValue_t Init(uint8_t pOffset);
	
	static void SendEOI(uint8_t pIntLine);
	
	static void Mask(uint8_t pIntLine);
	static void Unmask(uint8_t pIntLine);
	static void MaskAll(void);
	static void UnmaskAll(void);
	
	static uint8_t GetOffset(void);
	static uint8_t GetIntLineCount(void);
	
	static uint64_t GetSpuriousCount(void);
	
};

#endif
