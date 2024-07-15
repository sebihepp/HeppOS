
#include "console.hpp"

extern "C" video_font_t stdfont;

void *Console::Framebuffer = NULL;
uint32_t Console::Pitch = 0;
uint32_t Console::Width = 0;
uint32_t Console::Height = 0;

retval_t Console::Init(const multiboot2_info_t *mbi) {

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
	
	Framebuffer = (void*)(_mbiFramebufferTag->framebuffer_addr & 0xFFFFFFFF);
	Pitch = _mbiFramebufferTag->framebuffer_pitch;
	Width = _mbiFramebufferTag->framebuffer_width;
	Height = _mbiFramebufferTag->framebuffer_height;
	
	return RETVAL_OK;
}

void Console::PrintChar(uint8_t c, uint32_t x, uint32_t y,
	uint32_t fg_color, uint32_t bg_color)
{
	
	uint32_t *_framebuffer = (uint32_t*)Framebuffer;
	
	for (uint32_t row = 0; row < 16; row++) {
		for (uint32_t col = 0; col < 8; col++) {
			uint32_t _x = x + col;
			uint32_t _y = y + row;
			if (_x >= Width)
				continue;
			if (_y >= Height)
				continue;
				
			volatile uint32_t *target = _framebuffer + _y * (Pitch / 4) + _x;
			if (stdfont.a[c].a[row] & (0x80 >> col)) {				
				*target = fg_color;
			} else {
				*target = bg_color;
			}
		}
	}
}

void Console::PrintCharAlpha(uint8_t c, uint32_t x, uint32_t y, 
		uint32_t fg_color)
{
	uint32_t *_framebuffer = (uint32_t*)Framebuffer;
	
	for (uint32_t row = 0; row < 16; row++) {
		for (uint32_t col = 0; col < 8; col++) {
			uint32_t _x = x + col;
			uint32_t _y = y + row;
			if (_x >= Width)
				continue;
			if (_y >= Height)
				continue;
				
			volatile uint32_t *target = _framebuffer + _y * (Pitch / 4) + _x;
			if (stdfont.a[c].a[row] & (0x80 >> col)) {				
				*target = fg_color;
			}
		}
	}
}

uint32_t Console::GetWidth(void) {
	return Width;
}

uint32_t Console::GetHeight(void) {
	return Height;
}

void Console::Clear(uint32_t color) {
	uint32_t *_framebuffer = (uint32_t*)Framebuffer;
	
	for (uint32_t y = 0; y < Height; ++y) {
		for (uint32_t x = 0; x < Width; ++x) {
			volatile uint32_t *target = _framebuffer + y * (Pitch / 4) + x;
			*target = color;
		}
	}
}
