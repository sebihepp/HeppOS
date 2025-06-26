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

#ifndef PIC_HEADER
#define PIC_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <retval.h>
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
	CPIC() = delete;
	~CPIC() = delete;
	
	static uint8_t mOffset;
	static uint64_t mSpuriousCount;
	static uint16_t mMask;
	
public:
	static ReturnValue_t Init(uint8_t pOffset) __attribute__(( nothrow )) ;
	
	static void SendEOI(uint8_t pInt) __attribute__(( nothrow )) ;
	
	static void Mask(uint8_t pIRQ) __attribute__(( nothrow )) ;
	static void Unmask(uint8_t pIRQ) __attribute__(( nothrow )) ;
	
	static void MaskAll(void) __attribute__(( nothrow )) ;
	static void UnmaskAll(void) __attribute__(( nothrow )) ;
	
	static void SetMask(uint16_t pMask) __attribute__(( nothrow )) ;
	static uint16_t GetMask(void) __attribute__(( nothrow )) ;
	static void RestoreMask(void) __attribute__(( nothrow )) ;
	
	static bool CheckSpurious(uint8_t pInt) __attribute__(( nothrow )) ;
	static uint8_t GetOffset(void) __attribute__(( nothrow )) ;
	static uint8_t GetIntLineCount(void) __attribute__(( nothrow )) ;
	
	static uint64_t GetSpuriousCount(void) __attribute__(( nothrow )) ;
	
};

#endif

