
#include "pmm.hpp"
#include "console.hpp"


uint32_t PMM::Bitmap[PMM_BITMAP_SIZE];
	

void PMM::SetUsed(void *Address) {
	uint32_t _Page = (uint32_t)Address / 4096;
	uint32_t _ArrayIndex = _Page / 32;
	uint32_t _BitIndex = _Page % 32;
	
	Bitmap[_ArrayIndex] |= (1 << _BitIndex);
}

void PMM::SetUsedRange(void *Address, size_t size) {
	
	uintptr_t _EndAddress = (uintptr_t)Address + size;
	
	// Check for overflow
	if (_EndAddress < (uintptr_t)Address) {
		_EndAddress = 0xFFFFFFFF;
	}
	
	for (uintptr_t i = (uintptr_t)Address; i <= _EndAddress; i += 4096) {
		SetUsed((void*)i);
	}
	
}

void PMM::SetFree(void *Address) {
	uint32_t _Page = (uint32_t)Address / 4096;
	uint32_t _ArrayIndex = _Page / 32;
	uint32_t _BitIndex = _Page % 32;
	
	Bitmap[_ArrayIndex] &=  ~(1 << _BitIndex);
}

void PMM::SetFreeRange(void *Address, size_t size) {
	
	uintptr_t _EndAddress = (uintptr_t)Address + size;
	
	// Check for overflow
	if (_EndAddress < (uintptr_t)Address) {
		_EndAddress = 0xFFFFFFFF;
	}
	
	for (uintptr_t i = (uintptr_t)Address; i <= _EndAddress; i += 4096) {
		SetFree((void*)i);
	}
	
}
	
retval_t PMM::Init(multiboot2_info_t *mbi) {

	// First block every page
	for (uint32_t i = 0; i < PMM_BITMAP_SIZE; i++) {
		Bitmap[i] = 0xFFFFFFFF;
	}
	
	// Free every page marked as free in MemoryMap
	
	
	// Block every page marked as not free in MemoryMap
	
	
	// Block additional pages not in MemoryMap (like framebuffer, BDA, EBDA, etc.)
	
	// Block RealMode IVT
	SetUsedRange((void*)0x0, 1024);
	//Block BDA
	SetUsedRange((void*)0x400, 256);
	//Block EBDA
	uint16_t *_EBDAStartPointer = (uint16_t*)0x040E;
	uint32_t _EBDAStart = (uint32_t)(*_EBDAStartPointer);
	SetUsedRange((void*)_EBDAStart, 128*1024);
	//Block Video Memory
	SetUsedRange((void*)0xA0000, 128*1024);
	//Block Video BIOS
	SetUsedRange((void*)0xC0000, 32*1024);
	//Block BIOS Expansions
	SetUsedRange((void*)0xC8000, 160*1024);
	//Block Motherboard BIOS
	SetUsedRange((void*)0xF0000, 64*1024);
	
	//Block Framebuffer
	SetUsedRange(Console::GetFramebufferAddress(), Console::GetFramebufferSize());
	
	return RETVAL_OK;
}
