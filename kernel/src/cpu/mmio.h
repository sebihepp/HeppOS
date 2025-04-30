
#ifndef HEADER_MMIO
#define HEADER_MMIO

#include <stddef.h>
#include <stdint.h>

inline uint8_t mmio_inb(void *pAddress) __attribute__(( nothrow, always_inline ));
inline uint8_t mmio_inb(void *pAddress)
{
	return *reinterpret_cast<volatile uint8_t*>(pAddress);
}

inline uint16_t mmio_inw(void *pAddress) __attribute__(( nothrow, always_inline ));
inline uint16_t mmio_inw(void *pAddress)
{
	return *reinterpret_cast<volatile uint16_t*>(pAddress);
}

inline uint32_t mmio_inl(void *pAddress) __attribute__(( nothrow, always_inline ));
inline uint32_t mmio_inl(void *pAddress)
{
	return *reinterpret_cast<volatile uint32_t*>(pAddress);
}


inline void mmio_outb(void *pAddress, uint8_t pValue) __attribute__(( nothrow, always_inline ));
inline void mmio_outb(void *pAddress, uint8_t pValue)
{
	*reinterpret_cast<volatile uint8_t*>(pAddress) = pValue;
}

inline void mmio_outw(void *pAddress, uint16_t pValue) __attribute__(( nothrow, always_inline ));
inline void mmio_outw(void *pAddress, uint16_t pValue)
{
	*reinterpret_cast<volatile uint16_t*>(pAddress) = pValue;
}

inline void mmio_outl(void *pAddress, uint32_t pValue) __attribute__(( nothrow, always_inline ));
inline void mmio_outl(void *pAddress, uint32_t pValue)
{
	*reinterpret_cast<volatile uint32_t*>(pAddress) = pValue;
}

#endif

