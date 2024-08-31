
#include "console.hpp"

extern "C" video_font_t stdfont;

void (*Console::SetPixel)(uint32_t x, uint32_t y, uint32_t color) = NULL;
void (*Console::Fill)(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) = NULL;

void *Console::Framebuffer = NULL;
uint32_t Console::Pitch = 0;
uint32_t Console::Width = 0;
uint32_t Console::Height = 0;
uint8_t Console::BPC = 0;
bool Console::EGAMode = false;

uint32_t Console::FGColor = 0x00AAAAAA;
uint32_t Console::BGColor = 0x00000000;
uint32_t Console::TitleFGColor = 0x0000FFFF;
uint32_t Console::TitleBGColor = 0x000000AA;

const char *Console::Title = "";
uint32_t Console::TitleHeight = 16;

uint32_t Console::CursorX = 0;
uint32_t Console::CursorY = Console::TitleHeight;


retval_t Console::Init(const multiboot2_info_t *pMBInfo) {

	multiboot2_info_tag_framebuffer_t *_mbiFramebufferTag = (multiboot2_info_tag_framebuffer_t*)GetMultiboot2Tag(pMBInfo, MULTIBOOT2_TAG_TYPE_FRAMEBUFFER);
	if (_mbiFramebufferTag == NULL) {
		return RETVAL_ERROR_NO_FRAMEBUFFER;
	}
	
	Framebuffer = (void*)(_mbiFramebufferTag->framebuffer_addr & 0xFFFFFFFF);
	Pitch = _mbiFramebufferTag->framebuffer_pitch;
	Width = _mbiFramebufferTag->framebuffer_width;
	Height = _mbiFramebufferTag->framebuffer_height;
	BPC = _mbiFramebufferTag->framebuffer_bpp;
	
	if (_mbiFramebufferTag->framebuffer_type == MULTIBOOT2_FRAMEBUFFER_TYPE_COLOR) {
		
		EGAMode = false;
		TitleHeight = 16;
		switch (BPC) {
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
				Fill = NULL;
				return RETVAL_ERROR_VIDEOMODE;
		}	
		
	} else if (_mbiFramebufferTag->framebuffer_type == MULTIBOOT2_FRAMEBUFFER_TYPE_TEXT) {
		if (BPC != 16) {
			SetPixel = NULL;
			Fill = NULL;
			return RETVAL_ERROR_VIDEOMODE;
		}
		EGAMode = true;
		SetPixel = &SetPixelEGA;
		Fill = &FillEGA;
		TitleHeight = 1;
		BPC = 4;
	} else {
		return RETVAL_ERROR_VIDEOMODE;
	}

	return RETVAL_OK;
}
	
uint32_t Console::GetWidth(void) {
	return Width;
}

uint32_t Console::GetHeight(void) {
	return Height;
}

uint32_t Console::GetBPC(void) {
	return BPC;
}
	
bool Console::IsTextMode(void) {
	return EGAMode;	
}

void Console::PrintChar(const uint8_t c, uint32_t x, uint32_t y,
	uint32_t fg_color, uint32_t bg_color)
{
	
	if (EGAMode) {
		
		uint8_t _color = ((ConvertColorEGA(bg_color) & 0x7) << 4) | ConvertColorEGA(fg_color);
		volatile uint16_t *target = (uint16_t*)Framebuffer;
		target[y * Width + x] = c | (_color << 8);
		return;
		
	}
	
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
	
	
	if (EGAMode) {
		
		volatile uint16_t *target = (uint16_t*)Framebuffer;
		uint16_t _tmp = target[y * Pitch + x];
		
		uint8_t _color = (ConvertColorEGA(fg_color)) | ((_tmp >> 8) & 0x70);

		_tmp = c | (_color << 8);
		
		target[y * Width + x] = _tmp;
		
		return;
		
	}
	
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
	
	if (EGAMode) {

		uint32_t i = 0;
		uint32_t x = 1;
		while (Title[i] != 0) {
			PrintCharAlpha(Title[i], x, 0, TitleFGColor);
			i++;
			x += 1;
			if (x > Width)
				break;
		}
		
		return;
	}
	
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

	CursorX = 0;
	CursorY = TitleHeight;

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

void Console::SetTitleText(const char *pText) {
	Title = pText;
	
	// Background for Title
	Fill(0, 0, Width, TitleHeight, TitleBGColor);
	
	// Draw Title
	PrintTitle();
}

void Console::Print(const char *pText) {
	size_t i = 0;
	
	while (pText[i] != 0) {
		
		if (pText[i] == '\n') {
			if (EGAMode) {				
				CursorX = 0;
				CursorY += 1;
			} else {
				CursorX = 0;
				CursorY += 16;				
			}
			
			if (CursorY >= Height) {
				break;
			} else {
				i++;
				continue;
			}
		} else {
			PrintCharAlpha(pText[i], CursorX, CursorY, FGColor);
		}

		if (EGAMode) {				
			CursorX += 1;
			if (CursorX >= Width) {
				CursorX = 0;
				CursorY += 1;
			}
			if (CursorY >= Height) {
				break;
			}
		} else {
			CursorX += 8;
			if (CursorX >= Width) {
				CursorX = 0;
				CursorY += 16;
			}
			if (CursorY >= Height) {
				break;
			}
		}
		
		i++;
	}
	
}

void *Console::GetFramebufferAddress(void) {
	return Framebuffer;
}

size_t Console::GetFramebufferSize(void) {
	return Width * Pitch;
}

uint8_t Console::ConvertColorEGA(uint32_t color) {
	uint8_t _red = (color >> 16) & 0xFF;
	uint8_t _green = (color >> 8) & 0xFF;
	uint8_t _blue = color & 0xFF;

	uint8_t _result = 0;
	
	if (_red > 0xAA) {
		_result |= 0x0C;
	} else if (_red > 0x55) {
		_result |= 0x04;
	}
	if (_green > 0xAA) {
		_result |= 0x0A;
	} else if (_green > 0x55) {
		_result |= 0x02;
	}
	if (_blue > 0xAA) {
		_result |= 0x09;
	} else if (_blue > 0x55) {
		_result |= 0x01;
	}
		
	return _result;
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

void Console::SetPixelEGA(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= Width)
		return;
	if (y >= Height)
		return;
		
	uint8_t _EGAColor = ConvertColorEGA(color);
	
	volatile uint16_t *target = (uint16_t*)Console::Framebuffer;	
	target[y * Width + x] = 0xDB | (_EGAColor << 8);
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

void Console::FillEGA(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left >= Width)
		return;
	if (top >= Height)
		return;

	color = (ConvertColorEGA(color) & 0x7) << 4;
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= Width)
				continue;
			if (_y >= Height)
				continue;

			volatile uint16_t *target = (uint16_t*)Console::Framebuffer;
			target[_y * Width + _x] = ' ' | (color << 8);
		}
	}
}

void Console::Fill8(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left >= Width)
		return;
	if (top >= Height)
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
	if (left >= Width)
		return;
	if (top >= Height)
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
	if (left >= Width)
		return;
	if (top >= Height)
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
	if (left >= Width)
		return;
	if (top >= Height)
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
	if (left >= Width)
		return;
	if (top >= Height)
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
