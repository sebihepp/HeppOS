
#include "console.hpp"

extern "C" video_font_t stdfont;

void *Console::Framebuffer = NULL;
uint32_t Console::Pitch = 0;
uint32_t Console::Width = 0;
uint32_t Console::Height = 0;
uint8_t Console::BPP = 0;
	
uint32_t Console::FGColor = 0x00AAAAAA;
uint32_t Console::BGColor = 0x00000000;
uint32_t Console::TitleFGColor = 0x0000FFFF;
uint32_t Console::TitleBGColor = 0x000000AA;

const char *Console::Title = "";
const uint32_t Console::TitleHeight = 16;

uint32_t Console::CursorX = 0;
uint32_t Console::CursorY = Console::TitleHeight;

Console::Console() {
}

Console::~Console() {
	
}

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
	if (_mbiFramebufferTag->framebuffer_type != MULTIBOOT2_FRAMEBUFFER_TYPE_COLOR) {
		return RETVAL_ERROR_VIDEOMODE;
	}
	BPP = _mbiFramebufferTag->framebuffer_bpp;
	if (BPP != 32) {
		return RETVAL_ERROR_VIDEOMODE;
	}
	
	return RETVAL_OK;
}

void Console::PrintChar(const uint8_t c, uint32_t x, uint32_t y,
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

void Console::PrintCharAlpha(const uint8_t c, uint32_t x, uint32_t y, 
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

void Console::PrintTitle(void) {
	uint32_t i = 0;
	uint32_t x = 8;
	while (Title[i] != 0) {
		PrintCharAlpha(Title[i], x, 0, TitleFGColor);
		i++;
		x += 8;
		if (x > Width)
			break;
	}
}
	
void Console::Clear(void) {	
	// Clear all
	Fill(0, 0, Width, Height, BGColor);
	
	// Background for Title
	Fill(0, 0, Width, TitleHeight, TitleBGColor);
	
	// Draw Title
	PrintTitle();	
}

void Console::Fill(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) 
{
	uint32_t * _framebuffer = (uint32_t*)Framebuffer;
	if (left > Width)
		return;
	if (top > Height)
		return;	
	for (uint32_t y = top; y < bottom; y++) {
		for (uint32_t x = left; x < right; x++) {
			if (x >= Width)
				continue;
			if (y >= Height)
				continue;
			volatile uint32_t *_target = _framebuffer + y * (Pitch / 4) + x;
			*_target = color;
		}
	}
	
}

void Console::SetFGColor(uint32_t color) {
	FGColor = color;
}

void Console::SetBGColor(uint32_t color) {
	BGColor = color;
}

void Console::SetTitleFGColor(uint32_t color) {
	TitleFGColor = color;
	
	// Background for Title
	Fill(0, 0, Width, TitleHeight, TitleBGColor);
	
	// Draw Title
	PrintTitle();
}

void Console::SetTitleBGColor(uint32_t color) {
	TitleBGColor = color;
	
	// Background for Title
	Fill(0, 0, Width, TitleHeight, TitleBGColor);
	
	// Draw Title
	PrintTitle();
}

void Console::SetTitleText(const char *text) {
	Title = text;
	
	// Background for Title
	Fill(0, 0, Width, TitleHeight, TitleBGColor);
	
	// Draw Title
	PrintTitle();
}

void Console::Print(const char *text) {
	size_t i = 0;
	
	while (text[i] != 0) {
		
		if (text[i] == '\n') {
			CursorX = 0;
			CursorY += 16;
			if (CursorY >= Height) {
				break;
			} else {
				i++;
				continue;
			}
		} else {
			PrintCharAlpha(text[i], CursorX, CursorY, FGColor);
		}
		
		CursorX += 8;
		if (CursorX >= Width) {
			CursorX = 0;
			CursorY += 16;
		}
		if (CursorY >= Height) {
			break;
		}
		i++;
	}
	
}

void *Console::GetFramebuffer(void) {
	return Framebuffer;
}
