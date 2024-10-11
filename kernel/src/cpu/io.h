
#ifndef HEADER_IO
#define HEADER_IO

#include <stddef.h>
#include <stdint.h>

inline uint8_t inb(uint16_t pPort) {
	uint8_t _value = 0;
	asm volatile (
		"inb %%dx, %%al;\n"
		: "=a" (_value)
		: "d" (pPort)
		:
	);
	return _value;
}

inline uint16_t inw(uint16_t pPort) {
	uint16_t _value = 0;
	asm volatile (
		"inw %%dx, %%ax;\n"
		: "=a" (_value)
		: "d" (pPort)
		:
	);
	return _value;
}

inline uint32_t inl(uint16_t pPort) {
	uint32_t _value = 0;
	asm volatile (
		"inl %%dx, %%eax;\n"
		: "=a" (_value)
		: "d" (pPort)
		:
	);
	return _value;	
}


inline void outb(uint16_t pPort, uint8_t pValue) {
	asm volatile (
		"outb %%al, %%dx;\n"
		:
		: "a" (pValue), "d" (pPort)
		:
	);
}

inline void outw(uint16_t pPort, uint8_t pValue) {
		asm volatile (
		"outw %%ax, %%dx;\n"
		:
		: "a" (pValue), "d" (pPort)
		:
	);
}

inline void outl(uint16_t pPort, uint8_t pValue) {
		asm volatile (
		"outl %%eax, %%dx;\n"
		:
		: "a" (pValue), "d" (pPort)
		:
	);
}

#endif
