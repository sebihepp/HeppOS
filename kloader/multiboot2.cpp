
#include "multiboot2.hpp"

multiboot2_info_tag_t *GetMultiboot2Tag(const multiboot2_info_t *pMBInfo, const multiboot2_tag_type_t pType) {

	multiboot2_info_tag_t *_mbiCurrentTag = (multiboot2_info_tag_t *)((uintptr_t)pMBInfo + 8);
	
	//search for Multiboot2 tag type
	while ((uintptr_t)_mbiCurrentTag < ((uintptr_t)pMBInfo + pMBInfo->total_size)) {
		
		// Check for end of mbi tags
		if ((_mbiCurrentTag->type == MULTIBOOT2_TAG_TYPE_END) && (_mbiCurrentTag->size = 8)) {
			return NULL;
		}
		
		//Found Framebuffer Tag
		if (_mbiCurrentTag->type == pType) {
			return _mbiCurrentTag;
			break;
		}
		
		//otherwise jump to the next tag
		_mbiCurrentTag = (multiboot2_info_tag_t*)((uintptr_t)_mbiCurrentTag + _mbiCurrentTag->size);
		//Must be 8 byte aligned
		if ((uintptr_t)_mbiCurrentTag & 0x7) {
			_mbiCurrentTag = (multiboot2_info_tag_t*)((((uintptr_t)_mbiCurrentTag) + 8) & ~0x7);
		}
	}
	
	return NULL;
}
