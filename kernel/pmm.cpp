
#include "retvals.h"
#include "pmm.hpp"

uint32_t PMM::bitmapLow[PMM_BITMAP_LOW_SIZE];
uint32_t PMM::bitmapHigh[PMM_BITMAP_HIGH_SIZE];

uint32_t PMM::setBitmapValue(void *address, uint8_t value) {
	
	return RETVAL_ERROR;
}

uint32_t PMM::init(multiboot2_info_t *mbi) {
	
	multiboot2_info_tag_t *_mbiCurrentTag = (multiboot2_info_tag_t *)((uintptr_t)mbi + 8);
	multiboot2_info_tag_mmap_entry_t *_mbiMemoryMap = NULL;
	uint32_t _memoryMapEntryCount = 0;
	
	//search for Multiboot2 tag == memory map
	while ((uintptr_t)_mbiCurrentTag < ((uintptr_t)mbi + mbi->total_size)) {
		
		// Check for end of mbi tags
		if ((_mbiCurrentTag->type == 0) && (_mbiCurrentTag->size = 8)) {
			return RETVAL_ERROR;
		}
		
		//Found Memory Map Tag
		if (_mbiCurrentTag->type == 6) {
			_mbiMemoryMap = (multiboot2_info_tag_mmap_entry_t*)((uintptr_t)_mbiCurrentTag + 8);
			_memoryMapEntryCount = (((multiboot2_info_tag_mmap_t*)_mbiCurrentTag)->size - 8) / ((multiboot2_info_tag_mmap_t*)_mbiCurrentTag)->entry_size;
			break;
		}
		
		//otherwise jump to the next tag
		_mbiCurrentTag = (multiboot2_info_tag_t*)((uintptr_t)_mbiCurrentTag + _mbiCurrentTag->size);
		//Must be 8 byte aligned
		if ((uintptr_t)_mbiCurrentTag & 0x7) {
			_mbiCurrentTag = (multiboot2_info_tag_t*)((((uintptr_t)_mbiCurrentTag) + 8) & ~0x7);
		}
	}
	
	//Check if MemoryMap was found
	if (_mbiMemoryMap == NULL) {
		return RETVAL_ERROR;
	}
	
	//Set all pages to free
	for (uint32_t i = 0; i < PMM_BITMAP_LOW_SIZE; i++) {
		bitmapLow[i] = 0; 
	}
	// Mark Address 0x0 as used to prevent NULL pointer
	bitmapLow[0] = 0x1;
	
	//Set used pages to used
	for (uint32_t i = 0; i < _memoryMapEntryCount; i++) {
		
		//Ignore if start is above 1MB
		if (_mbiMemoryMap[i].base_addr > 1024*1024)
			continue;
		
		//If RAM is available, check next entry
		if (_mbiMemoryMap[i].type == 1)
			continue;
		
		//Set Pages to used
		for (uint64_t _address = _mbiMemoryMap[i].base_addr; _address < (_mbiMemoryMap[i].base_addr + _mbiMemoryMap[i].length); _address += PMM_PAGE_SIZE) {
			
			//Ignore addresses over 1MB
			if (_address > 1024*1024)
				break;
			
			//Calculate byte and bit
			uint32_t _pageNumber = _address / 4096;
			uint32_t _byte = _pageNumber / (sizeof(pmm_bitmap_t) * 8);
			uint32_t _bit = _pageNumber % (sizeof(pmm_bitmap_t) * 8);
			
			bitmapLow[_byte] |= 0x1 << _bit;
		}
		
	}
	
	return RETVAL_OK;
}

uint32_t PMM::allocPage(void* &address) {
	
	address = (void*)NULL;
	return RETVAL_ERROR;
}

uint32_t PMM::allocPages(uint32_t numPages, void* &address) {

	address = (void*)NULL;
	return RETVAL_ERROR;
}
	
uint32_t PMM::allocLowPage(void* &address) {
	
	for (uint32_t i = 0; i < PMM_BITMAP_LOW_SIZE; i++) {
		if (bitmapLow[i] == 0xFFFFFFFF)
			continue;
		
		for (uint8_t s = 0; s < (sizeof(pmm_bitmap_t) * 8); s++) {
			if (((bitmapLow[i] >> s) & 0x1) == 0) {
				address = (void*)((i * sizeof(pmm_bitmap_t) * 8 + s) * PMM_PAGE_SIZE);
				return RETVAL_OK;
			}
		}
	}
	
	address = (void*)NULL;
	return RETVAL_ERROR;
}

uint32_t PMM::allocLowPages(uint32_t numPages, void* &address) {

	address = (void*)NULL;
	return RETVAL_ERROR;
}

uint32_t PMM::freePage(void *address) {
	
	return RETVAL_ERROR;
}

uint32_t PMM::freePages(void *address, uint32_t numPages) {
	
	return RETVAL_ERROR;
}
