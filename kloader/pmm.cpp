
#include "pmm.hpp"

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
	
	
	return RETVAL_OK;
}
