
#include "pmm.hpp"

uint8_t PMM::Bitmap[PMM_BITMAP_SIZE];
	

retval_t PMM::Init(multiboot2_info_t *mbi) {

	// First block every page
	for (uint32_t i = 0; i < PMM_BITMAP_SIZE; i++) {
		Bitmap[i] = 0xFF;
	}
	
	// Free every page marked as free in MemoryMap
	
	
	// Block every page marked as not free in MemoryMap
	
	
	// Block additional pages not in MemoryMap (like framebuffer, BDA, EBDA, etc.)
	
	
	return RETVAL_OK;
}
