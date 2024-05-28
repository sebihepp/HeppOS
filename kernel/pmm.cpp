
#include "retvals.h"
#include "pmm.hpp"

uint32_t PMM::bitmapLow[PMM_BITMAP_LOW_SIZE];

struct mbi_tag {
	uint32_t type;
	uint32_t size;
	uint8_t data;
} __attribute__((packed));

struct mbi_tag_memory_map {
	uint32_t type;
	uint32_t size;
	uint32_t entry_size;
	uint32_t entry_version;
	uint8_t data;
} __attribute__((packed));

struct mbi_memory_map_entry {
	uint64_t base_address;
	uint64_t length;
	uint32_t type;
	uint32_t reserved;
} __attribute__((packed));

struct mbi_header {
	uint32_t total_size;
	uint32_t reserved;
	uint8_t data;
} __attribute__((packed));

uint32_t PMM::init(void *mbi) {
	
	mbi_header *_mbiHeader = (mbi_header*)mbi;
	mbi_tag *_mbiCurrentTag = (mbi_tag*)&(_mbiHeader->data);
	mbi_memory_map_entry *_mbiMemoryMap = NULL;
	uint32_t _memoryMapEntryCount = 0;
	
	//search for Multiboot2 tag == memory map
	while ((uintptr_t)_mbiCurrentTag < ((uintptr_t)_mbiHeader + _mbiHeader->total_size)) {
		
		// Check for end of mbi tags
		if ((_mbiCurrentTag->type == 0) && (_mbiCurrentTag->size = 8)) {
			return RETVAL_ERROR;
		}
		
		//Found Memory Map Tag
		if (_mbiCurrentTag->type == 6) {
			_mbiMemoryMap = (mbi_memory_map_entry*)&(((mbi_tag_memory_map*)_mbiCurrentTag)->data);
			_memoryMapEntryCount = (((mbi_tag_memory_map*)_mbiCurrentTag)->size - 8) / ((mbi_tag_memory_map*)_mbiCurrentTag)->entry_size;
			break;
		}
		
		//otherwise jump to the next tag
		_mbiCurrentTag = (mbi_tag*)((uintptr_t)_mbiCurrentTag + _mbiCurrentTag->size);
		//Must be 8 byte aligned
		if ((uintptr_t)_mbiCurrentTag & 0x7) {
			_mbiCurrentTag = (mbi_tag*)((((uintptr_t)_mbiCurrentTag) + 8) & ~0x7);
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
		if (_mbiMemoryMap[i].base_address > 1024*1024)
			continue;
		
		//If RAM is available, check next entry
		if (_mbiMemoryMap[i].type == 1)
			continue;
		
		//Set Pages to used
		for (uint64_t _address = _mbiMemoryMap[i].base_address; _address < (_mbiMemoryMap[i].base_address + _mbiMemoryMap[i].length); _address += PMM_PAGE_SIZE) {
			
			//Ignore addresses over 1MB
			if (_address > 1024*1024)
				break;
			
			//Calculate byte and bit
			uint32_t _pageNumber = _address / 4096;
			uint32_t _byte = _pageNumber / (sizeof(pmm_bitmap_type) * 8);
			uint32_t _bit = _pageNumber % (sizeof(pmm_bitmap_type) * 8);
			
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
		
		for (uint8_t s = 0; s < (sizeof(pmm_bitmap_type) * 8); s++) {
			if (((bitmapLow[i] >> s) & 0x1) == 0) {
				address = (void*)((i * sizeof(pmm_bitmap_type) * 8 + s) * PMM_PAGE_SIZE);
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
