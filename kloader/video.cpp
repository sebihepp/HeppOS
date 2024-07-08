
#include "video.hpp"

extern "C" video_font_t stdfont;

void *Video::framebuffer = NULL;
uint32_t Video::pitch = 0;
uint32_t Video::width = 0;
uint32_t Video::height = 0;

//For Testing purposes
uint32_t fg_color = 0x00C0C0C0;
uint32_t bg_color = 0x00000000;

retval_t Video::init(multiboot2_info_t *mbi) {

	multiboot2_info_tag_t *_mbiCurrentTag = (multiboot2_info_tag_t *)((uintptr_t)mbi + 8);
	multiboot2_info_tag_framebuffer_t *_mbiFramebufferTag = NULL;
	
	
	//search for Multiboot2 tag == framebuffer
	while ((uintptr_t)_mbiCurrentTag < ((uintptr_t)mbi + mbi->total_size)) {
		
		// Check for end of mbi tags
		if ((_mbiCurrentTag->type == 0) && (_mbiCurrentTag->size = 8)) {
			return RETVAL_ERROR_NO_FRAMEBUFFER;
		}
		
		//Found Framebuffer Tag
		if (_mbiCurrentTag->type == MULTIBOOT2_TAG_TYPE_FRAMEBUFFER) {
			_mbiFramebufferTag = (multiboot2_info_tag_framebuffer_t*)_mbiCurrentTag;
			break;
		}
		
		//otherwise jump to the next tag
		_mbiCurrentTag = (multiboot2_info_tag_t*)((uintptr_t)_mbiCurrentTag + _mbiCurrentTag->size);
		//Must be 8 byte aligned
		if ((uintptr_t)_mbiCurrentTag & 0x7) {
			_mbiCurrentTag = (multiboot2_info_tag_t*)((((uintptr_t)_mbiCurrentTag) + 8) & ~0x7);
		}
	}
	
	if ((uintptr_t)_mbiCurrentTag >= ((uintptr_t)mbi + mbi->total_size)) {
		return RETVAL_ERROR_NO_FRAMEBUFFER;
	}
	
	framebuffer = (void*)(_mbiFramebufferTag->framebuffer_addr & 0xFFFFFFFF);
	pitch = _mbiFramebufferTag->framebuffer_pitch;
	width = _mbiFramebufferTag->framebuffer_width;
	height = _mbiFramebufferTag->framebuffer_height;
	
	return RETVAL_OK;
}

void Video::print_char(uint8_t c, uint32_t x, uint32_t y) {
	
	uint32_t *_framebuffer = (uint32_t*)framebuffer;
	
	for (uint32_t row = 0; row < 16; row++) {
		for (uint32_t col = 0; col < 8; col++) {
			uint32_t _x = x + col;
			uint32_t _y = y + row;
			if (_x >= width)
				continue;
			if (_y >= height)
				continue;
				
			volatile uint32_t *target = _framebuffer + _y * (pitch / 4) + _x;
			if (stdfont.a[c].a[row] & (0x80 >> col)) {				
				*target = fg_color;
			} else {
				*target = bg_color;
			}
		}
	}
}

uint32_t Video::getWidth(void) {
	return width;
}

uint32_t Video::getHeight(void) {
	return height;
}

void Video::clear(uint32_t color) {
	uint32_t *_framebuffer = (uint32_t*)framebuffer;
	
	for (uint32_t y = 0; y < height; ++y) {
		for (uint32_t x = 0; x < width; ++x) {
			volatile uint32_t *target = _framebuffer + y * (pitch / 4) + x;
			*target = color;
		}
	}
}
