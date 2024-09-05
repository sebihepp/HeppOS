
#include "console.hpp"

extern "C" video_font_t stdfont;

void *Console::mFramebuffer = NULL;
uint32_t Console::mPitch = 0;
uint32_t Console::mWidth = 0;
uint32_t Console::mHeight = 0;
uint8_t Console::mBPC = 0;
bool Console::mEGAMode = false;

uint32_t Console::mFGColor = 0x00AAAAAA;
uint32_t Console::mBGColor = 0x00000000;
uint32_t Console::mTitleFGColor = 0x0000FFFF;
uint32_t Console::mTitleBGColor = 0x000000AA;

const char *Console::mTitle = "";
uint32_t Console::mTitleHeight = 1;

uint32_t Console::mCursorX = 0;
uint32_t Console::mCursorY = 0;
uint32_t Console::mCursorWidth = 0;
uint32_t Console::mCursorHeight = 0;
uint32_t Console::mCursorMaxX = 0;
uint32_t Console::mCursorMaxY = 0;


void (*Console::mSetPixel)(uint32_t x, uint32_t y, uint32_t color) = NULL;
void (*Console::mFill)(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) = NULL;
	
	
retval_t Console::Init(const multiboot2_info_t *pMBInfo) {

	multiboot2_info_tag_framebuffer_t *_mbiFramebufferTag = (multiboot2_info_tag_framebuffer_t*)GetMultiboot2Tag(pMBInfo, MULTIBOOT2_TAG_TYPE_FRAMEBUFFER);
	if (_mbiFramebufferTag == NULL) {
		return RETVAL_ERROR_NO_FRAMEBUFFER;
	}
	
	mFramebuffer = (void*)(_mbiFramebufferTag->framebuffer_addr & 0xFFFFFFFF);
	mPitch = _mbiFramebufferTag->framebuffer_pitch;
	mWidth = _mbiFramebufferTag->framebuffer_width;
	mHeight = _mbiFramebufferTag->framebuffer_height;
	mBPC = _mbiFramebufferTag->framebuffer_bpp;
	
	if (_mbiFramebufferTag->framebuffer_type == MULTIBOOT2_FRAMEBUFFER_TYPE_COLOR) {
		
		mCursorX = 0;
		mCursorY = 0;
		mCursorWidth = 8;
		mCursorHeight = 16;
		mCursorMaxX = mWidth / mCursorWidth;
		mCursorMaxY = mHeight / mCursorHeight;
		
		mEGAMode = false;
		mTitleHeight = 1;
		switch (mBPC) {
			case 32:
				mSetPixel = &SetPixel32;
				mFill = &Fill32;
				break;
			case 24:
				mSetPixel = &SetPixel24;
				mFill = &Fill24;
				break;
			case 16:
				mSetPixel = &SetPixel16;
				mFill = &Fill16;
				break;
			case 15:
				mSetPixel = &SetPixel15;
				mFill = &Fill15;
				break;
			case 8:
				mSetPixel = &SetPixel8;
				mFill = &Fill8;
				break;
				
			default:
				mSetPixel = NULL;
				mFill = NULL;
				return RETVAL_ERROR_VIDEOMODE;
		}
		
	} else if (_mbiFramebufferTag->framebuffer_type == MULTIBOOT2_FRAMEBUFFER_TYPE_TEXT) {
		if (mBPC != 16) {
			mSetPixel = NULL;
			mFill = NULL;
			return RETVAL_ERROR_VIDEOMODE;
		}
		
		mCursorX = 0;
		mCursorY = 0;
		mCursorWidth = 1;
		mCursorHeight = 1;
		mCursorMaxX = mWidth;
		mCursorMaxY = mHeight;
		
		mEGAMode = true;
		mSetPixel = &SetPixelEGA;
		mFill = &FillEGA;
		mTitleHeight = 1;
		mBPC = 4;
	} else {
		return RETVAL_ERROR_VIDEOMODE;
	}

	return RETVAL_OK;
}
	
uint32_t Console::GetWidth(void) {
	return mWidth;
}

uint32_t Console::GetHeight(void) {
	return mHeight;
}

uint32_t Console::GetBPC(void) {
	return mBPC;
}
	
bool Console::IsTextMode(void) {
	return mEGAMode;	
}

void Console::PrintChar(const uint8_t c, uint32_t x, uint32_t y,
	uint32_t fg_color, uint32_t bg_color)
{
	
	x *= mCursorWidth;
	y *= mCursorHeight;
	
	if (mEGAMode) {
		
		if (x >= mWidth)
			return;
		if (y >= mHeight)
			return;


		uint8_t _color = ((ConvertColorEGA(bg_color) & 0x7) << 4) | ConvertColorEGA(fg_color);
		volatile uint16_t *target = (uint16_t*)mFramebuffer;
		target[y * mPitch + x] = c | (_color << 8);
		return;
		
	}
	
	for (uint32_t row = 0; row < 16; row++) {
		for (uint32_t col = 0; col < 8; col++) {
			uint32_t _x = x + col;
			uint32_t _y = y + row;
			if (_x >= mWidth)
				continue;
			if (_y >= mHeight)
				continue;
				
			if (stdfont.a[c].a[row] & (0x80 >> col)) {				
				mSetPixel(_x, _y, fg_color);
			} else {
				mSetPixel(_x, _y, bg_color);
			}
		}
	}
}

void Console::PrintCharAlpha(const uint8_t c, uint32_t x, uint32_t y, 
		uint32_t fg_color)
{
	
	x *= mCursorWidth;
	y *= mCursorHeight;
	
	if (mEGAMode) {
	
		if (x >= mWidth)
			return;
		if (y >= mHeight)
			return;
		
		
		volatile uint16_t *target = (uint16_t*)mFramebuffer;
		uint16_t _tmp = target[y * mPitch + x];
		
		uint8_t _color = (ConvertColorEGA(fg_color)) | ((_tmp >> 8) & 0x70);

		_tmp = c | (_color << 8);
		
		target[y * mWidth + x] = _tmp;
		
		return;
		
	}
	
	for (uint32_t row = 0; row < 16; row++) {
		for (uint32_t col = 0; col < 8; col++) {
			uint32_t _x = x + col;
			uint32_t _y = y + row;
			if (_x >= mWidth)
				continue;
			if (_y >= mHeight)
				continue;
				
			if (stdfont.a[c].a[row] & (0x80 >> col)) {				
				SetPixel(_x, _y, fg_color);
			}
		}
	}
}

void Console::PrintTitle(void) {
	
	if (mEGAMode) {

		uint32_t i = 0;
		uint32_t x = 1;
		while (mTitle[i] != 0) {
			PrintCharAlpha(mTitle[i], x, 0, mTitleFGColor);
			i++;
			x += 1;
			if (x > mWidth)
				break;
		}
		
		return;
	}
	
	uint32_t i = 0;
	uint32_t x = 1;
	while (mTitle[i] != 0) {
		PrintCharAlpha(mTitle[i], x, 0, mTitleFGColor);
		i++;
		x += 1;
		if (x > mWidth)
			break;
	}
}
	
void Console::Clear(void) {	
	
	// Clear all
	Fill(0, 0, mWidth, mHeight, mBGColor);
	
	// Background for Title
	Fill(0, 0, mWidth, mTitleHeight * mCursorHeight, mTitleBGColor);
	
	// Draw Title
	PrintTitle();

	mCursorX = 0;
	mCursorY = mTitleHeight;

}

void Console::SetFGColor(uint32_t color) {
	mFGColor = color;
}

void Console::SetBGColor(uint32_t color) {
	mBGColor = color;
}

void Console::SetTitleFGColor(uint32_t color) {
	mTitleFGColor = color;
	
	// Background for Title
	Fill(0, 0, mWidth, mTitleHeight, mTitleBGColor);
	
	// Draw Title
	PrintTitle();
}

void Console::SetTitleBGColor(uint32_t color) {
	mTitleBGColor = color;
	
	// Background for Title
	Fill(0, 0, mWidth, mTitleHeight, mTitleBGColor);
	
	// Draw Title
	PrintTitle();
}

void Console::SetTitleText(const char *pText) {
	mTitle = pText;
	
	// Background for Title
	Fill(0, 0, mWidth, mTitleHeight, mTitleBGColor);
	
	// Draw Title
	PrintTitle();
}

void Console::Print(const char *pText) {
	size_t i = 0;
	
	while (pText[i] != 0) {
		
		if (pText[i] == '\n') {
			mCursorX = 0;
			mCursorY += 1;
			
			if (mCursorY >= mCursorMaxY) {
				break;
			} else {
				i++;
				continue;
			}
		} else {
			PrintCharAlpha(pText[i], mCursorX, mCursorY, mFGColor);
		}

		mCursorX += 1;
		if (mCursorX >= mCursorMaxX) {
			mCursorX = 0;
			mCursorY += 1;
		}
		if (mCursorY >= mCursorMaxY) {
			break;
		}
		
		i++;
	}
	
}

void *Console::GetFramebufferAddress(void) {
	return mFramebuffer;
}

size_t Console::GetFramebufferSize(void) {
	return mWidth * mPitch;
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
	if (x >= mWidth)
		return;
	if (y >= mHeight)
		return;
		
	uint8_t _EGAColor = ConvertColorEGA(color);
	
	volatile uint16_t *target = (uint16_t*)mFramebuffer;	
	target[y * mWidth + x] = 0xDB | (_EGAColor << 8);
}

void Console::SetPixel8(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= mWidth)
		return;
	if (y >= mHeight)
		return;
		
	color = ConvertColor8(color);
	
	volatile uint8_t *target = (uint8_t*)(((uintptr_t)mFramebuffer) + (y * mPitch) + (x));
	*target = color;
}

void Console::SetPixel15(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= mWidth)
		return;
	if (y >= mHeight)
		return;
		
	color = ConvertColor15(color);
	
	volatile uint16_t *target = (uint16_t*)(((uintptr_t)mFramebuffer) + (y * mPitch) + (x * 2));
	*target = color;
}

void Console::SetPixel16(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= mWidth)
		return;
	if (y >= mHeight)
		return;
		
	color = ConvertColor16(color);
	
	volatile uint16_t *target = (uint16_t*)(((uintptr_t)mFramebuffer) + (y * mPitch) + (x * 2));	
	*target = color;
}

void Console::SetPixel24(uint32_t x, uint32_t y, uint32_t color) {
	
	if (x >= mWidth)
		return;
	if (y >= mHeight)
		return;
	
	color = ConvertColor24(color);
	
	volatile uint32_t *target = (uint32_t*)(((uintptr_t)mFramebuffer) + (y * mPitch) + (x * 3));
	uint32_t tmp = *target & 0xFF000000;
	tmp |= color;
	*target = color;
}

void Console::SetPixel32(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= mWidth)
		return;
	if (y >= mHeight)
		return;
		
	color = ConvertColor32(color);
	
	volatile uint32_t *target = (uint32_t*)(((uintptr_t)mFramebuffer) + (y * mPitch) + (x * 4));
	*target = color;
}

void Console::FillEGA(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left >= mWidth)
		return;
	if (top >= mHeight)
		return;

	color = (ConvertColorEGA(color) & 0x7) << 4;
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= mWidth)
				continue;
			if (_y >= mHeight)
				continue;

			volatile uint16_t *target = (uint16_t*)mFramebuffer;
			target[_y * mWidth + _x] = ' ' | (color << 8);
		}
	}
}

void Console::Fill8(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left >= mWidth)
		return;
	if (top >= mHeight)
		return;	

	color = ConvertColor8(color);
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= mWidth)
				continue;
			if (_y >= mHeight)
				continue;

			volatile uint8_t *target = (uint8_t*)(((uintptr_t)mFramebuffer) + (_y * mPitch) + (_x));
			*target = color;
		}
	}
}

void Console::Fill15(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left >= mWidth)
		return;
	if (top >= mHeight)
		return;	

	color = ConvertColor15(color);
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= mWidth)
				continue;
			if (_y >= mHeight)
				continue;

			volatile uint16_t *target = (uint16_t*)(((uintptr_t)mFramebuffer) + (_y * mPitch) + (_x * 2));
			*target = color;
		}
	}
}

void Console::Fill16(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left >= mWidth)
		return;
	if (top >= mHeight)
		return;	

	color = ConvertColor16(color);
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= mWidth)
				continue;
			if (_y >= mHeight)
				continue;

			volatile uint16_t *target = (uint16_t*)(((uintptr_t)mFramebuffer) + (_y * mPitch) + (_x * 2));
			*target = color;
		}
	}
}

void Console::Fill24(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left >= mWidth)
		return;
	if (top >= mHeight)
		return;	

	color = ConvertColor24(color);
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= mWidth)
				continue;
			if (_y >= mHeight)
				continue;

			volatile uint32_t *target = (uint32_t*)(((uintptr_t)mFramebuffer) + (_y * mPitch) + (_x * 3));
			uint32_t tmp = *target & 0xFF000000;
			tmp |= color;
			*target = color;
		}
	}
}

void Console::Fill32(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	if (left >= mWidth)
		return;
	if (top >= mHeight)
		return;	

	color = ConvertColor32(color);
	
	for (uint32_t _y = top; _y < bottom; _y++) {
		for (uint32_t _x = left; _x < right; _x++) {
			if (_x >= mWidth)
				continue;
			if (_y >= mHeight)
				continue;

			volatile uint32_t *target = (uint32_t*)(((uintptr_t)mFramebuffer) + (_y * mPitch) + (_x * 4));
			*target = color;
		}
	}
}

void Console::SetPixel(uint32_t x, uint32_t y, uint32_t color) {
	mSetPixel(x, y, color);
}

void Console::Fill(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	mFill(left, top, right, bottom, color);
}

