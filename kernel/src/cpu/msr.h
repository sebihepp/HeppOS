
#ifndef MSR_HEADER
#define MSR_HEADER

#include <stddef.h>
#include <stdint.h>

#define MSR_PAT (0x277)
#define MSR_EFER (0xC0000080)

inline void ReadMSR(uint32_t pMSR, uint32_t *pLow, uint32_t *pHigh) __attribute__(( nothrow, always_inline ));
inline void ReadMSR(uint32_t pMSR, uint32_t *pLow, uint32_t *pHigh)
{
	asm volatile 
	(
		"mov %2, %%ecx;\n"
		"rdmsr;\n"
		: "=a" (*pLow), "=d" (*pHigh)
		: "m" (pMSR)
		: "ecx"
	);

}

inline void ReadMSR(uint32_t pMSR, uint64_t *pData) __attribute__(( nothrow, always_inline ));
inline void ReadMSR(uint32_t pMSR, uint64_t *pData)
{
	uint32_t _Low = 0;
	uint32_t _High = 0;
	ReadMSR(pMSR, &_Low, &_High);
	*pData = static_cast<uint64_t>(_Low);
	*pData |= static_cast<uint64_t>(_High) << 32;
}

inline void WriteMSR(uint32_t pMSR, uint32_t pLow, uint32_t pHigh) __attribute__(( nothrow, always_inline ));
inline void WriteMSR(uint32_t pMSR, uint32_t pLow, uint32_t pHigh)
{
	asm volatile 
	(
		"mov %0, %%eax;\n"
		"mov %1, %%edx;\n"
		"mov %2, %%ecx;\n"
		"wrmsr;\n"
		:
		: "m" (pLow), "m" (pHigh), "m" (pMSR)
		: "eax", "ecx", "edx"
	);	
}

inline void WriteMSR(uint32_t pMSR, uint64_t *pData) __attribute__(( nothrow, always_inline ));
inline void WriteMSR(uint32_t pMSR, uint64_t *pData)
{
	uint32_t _Low = *pData & 0xFFFFFFFF;
	uint32_t _High = (*pData >> 32) & 0xFFFFFFFF;
	WriteMSR(pMSR, _Low, _High);
}


#endif

