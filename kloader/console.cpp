
#include "console.hpp"

extern "C" video_font_t stdfont;

void (*Console::SetPixel)(uint32_t x, uint32_t y, uint32_t color) = NULL;
void (*Console::Fill)(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) = NULL;

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
	switch (BPP) {
		case 32:
			SetPixel = &SetPixel32;
			Fill = &Fill32;
			break;
		case 24:
			SetPixel = &SetPixel24;
			Fill = &Fill24;
			break;
		case 16:
			SetPixel = &SetPixel16;
			Fill = &Fill16;
			break;
		case 15:
			SetPixel = &SetPixel15;
			Fill = &Fill15;
			break;
		case 8:
			SetPixel = &SetPixel8;
			Fill = &Fill8;
			break;
			
		default:
			SetPixel = NULL;
			return RETVAL_ERROR_VIDEOMODE;
	}	
	return RETVAL_OK;
}

void Console::PrintChar(const uint8_t c, uint32_t x, uint32_t y,
	uint32_t fg_color, uint32_t bg_color)
{
	
	for (uint32_t row = 0; row < 16; row++) {
		for (uint32_t col = 0; col < 8; col++) {
			uint32_t _x = x + col;
			uint32_t _y = y + row;
			if (_x >= Width)
				continue;
			if (_y >= Height)
				continue;
				
			if (stdfont.a[c].a[row] & (0x80 >> col)) {				
				SetPixel(_x, _y, fg_color);
			} else {
				SetPixel(_x, _y, bg_color);
			}
		}
	}
}

void Console::PrintCharAlpha(const uint8_t c, uint32_t x, uint32_t y, 
		uint32_t fg_color)
{
	
	for (uint32_t row = 0; row < 16; row++) {
		for (uint32_t col = 0; col < 8; col++) {
			uint32_t _x = x + col;
			uint32_t _y = y + row;
			if (_x >= Width)
				continue;
			if (_y >= Height)
				continue;
				
			if (stdfont.a[c].a[row] & (0x80 >> col)) {				
				SetPixel(_x, _y, fg_color);
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

uint8_t Console::ConvertColor8(uint32_t color) {
	uint8_t _red = (color >> 16) & 0xFF;
	uint8_t _green = (color >> 8) & 0xFF;
	uint8_t _blue = color & 0xFF;
	
	// Highest bits only
	_red >>= 5;
	_green >>= 5;
	_blue >>= 6;
	
	uint8_t _result = 0;
	_result = _blue;
	_result |= _green << 2;
	_result |= _red << 5;
	
	return _result;
}

uint16_t Console::ConvertColor15(uint32_t color) {
	
	uint8_t _red = (color >> 16) & 0xFF;
	uint8_t _green = (color >> 8) & 0xFF;
	uint8_t _blue = color & 0xFF;
	
	// Highest bits only
	_red >>= 3;
	_green >>= 3;
	_blue >>= 3;
	
	uint16_t _result = 0;
	_result |= _blue;
	_result |= _green << 5;
	_result |= _red  << 10;
	
	return _result;
}

uint16_t Console::ConvertColor16(uint32_t color) {
	
	uint8_t _red = (color >> 16) & 0xFF;
	uint8_t _green = (color >> 8) & 0xFF;
	uint8_t _blue = color & 0xFF;
	
	// Highest bits only
	_red >>= 3;
	_green >>= 2;
	_blue >>= 3;
	
	uint16_t _result = 0;
	_result |= _blue;
	_result |= _green << 5;
	_result |= _red  << 11;
	
	return _result;
}

uint32_t Console::ConvertColor24(uint32_t color) {	
	return color & 0x00FFFFFF;
}

uint32_t Console::ConvertColor32(uint32_t color) {	
	return color;
}

void Console::SetPixel8(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= Width)
		return;
	if (y >= Height)
		return;
		
	color = ConvertColor8(color);
	
	volatile uint8_t *target = (uint8_t*)(((uintptr_t)Console::Framebuffer) + (y * Pitch) + (x));
	*target = color;
}

void Console::SetPixel15(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= Width)
		return;
	if (y >= Height)
		return;
		
	color = ConvertColor15(color);
	
	volatile uint16_t *target = (uint16_t*)(((uintptr_t)Console::Framebuffer) + (y * Pitch) + (x * 2));
	*target = color;
}

void Console::SetPixel16(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= Width)
		return;
	if (y >= Height)
		return;
		
	color = ConvertColor16(color);
	
	volatile uint16_t *target = (uint16_t*)(((uintptr_t)Console::Framebuffer) + (y * Pitch) + (x * 2));	
	*target = color;
}

void Console::SetPixel24(uint32_t x, uint32_t y, uint32_t color) {
	
	if (x >= Width)
		return;
	if (y >= Height)
		return;
	
	color = ConvertColor24(color);
	
	volatile uint32_t *target = (uint32_t*)(((uintptr_t)Console::Framebuffer) + (y * Pitch) + (x * 3));
	uint32_t tmp = *target & 0xFF000000;
	tmp |= color;
	*target = color;
}

void Console::SetPixel32(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= Width)
		return;
	if (y >= Height)
		return;
		
	color = ConvertColor32(color);
	
	volatile uint32_t *target = (uint32_t*)(((uintptr_t)Console::Framebuffer) + (y * Pitch) + (x * 4));
	*target = color;
}

void Console::Fill8(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left > Width)
		return;
	if (top > Height)
		return;	

	color = ConvertColor8(color);
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= Width)
				continue;
			if (_y >= Height)
				continue;

			volatile uint8_t *target = (uint8_t*)(((uintptr_t)Console::Framebuffer) + (_y * Pitch) + (_x));
			*target = color;
		}
	}
}

void Console::Fill15(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left > Width)
		return;
	if (top > Height)
		return;	

	color = ConvertColor15(color);
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= Width)
				continue;
			if (_y >= Height)
				continue;

			volatile uint16_t *target = (uint16_t*)(((uintptr_t)Console::Framebuffer) + (_y * Pitch) + (_x * 2));
			*target = color;
		}
	}
}

void Console::Fill16(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left > Width)
		return;
	if (top > Height)
		return;	

	color = ConvertColor16(color);
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= Width)
				continue;
			if (_y >= Height)
				continue;

			volatile uint16_t *target = (uint16_t*)(((uintptr_t)Console::Framebuffer) + (_y * Pitch) + (_x * 2));
			*target = color;
		}
	}
}

void Console::Fill24(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left > Width)
		return;
	if (top > Height)
		return;	

	color = ConvertColor24(color);
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= Width)
				continue;
			if (_y >= Height)
				continue;

			volatile uint32_t *target = (uint32_t*)(((uintptr_t)Console::Framebuffer) + (_y * Pitch) + (_x * 3));
			uint32_t tmp = *target & 0xFF000000;
			tmp |= color;
			*target = color;
		}
	}
}

void Console::Fill32(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left > Width)
		return;
	if (top > Height)
		return;	

	color = ConvertColor32(color);
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= Width)
				continue;
			if (_y >= Height)
				continue;

			volatile uint32_t *target = (uint32_t*)(((uintptr_t)Console::Framebuffer) + (_y * Pitch) + (_x * 4));
			*target = color;
		}
	}
}
