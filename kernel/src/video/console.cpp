
#include <video/console.h>
#include <cstub.h>

extern "C" VideoFont_t STDFont;

void *CConsole::mFramebuffer = NULL;
uint32_t CConsole::mPitch = 0;
uint32_t CConsole::mWidth = 0;
uint32_t CConsole::mHeight = 0;
uint8_t CConsole::mBPP = 0;

uint32_t CConsole::mFGColor = 0x00AAAAAA;
uint32_t CConsole::mBGColor = 0x00000000;
uint32_t CConsole::mTitleFGColor = 0x0000FFFF;
uint32_t CConsole::mTitleBGColor = 0x000000AA;

const char *CConsole::mTitle = "";
uint32_t CConsole::mTitleHeight = 1;

uint32_t CConsole::mCursorX = 0;
uint32_t CConsole::mCursorY = 0;
uint32_t CConsole::mCursorWidth = 0;
uint32_t CConsole::mCursorHeight = 0;
uint32_t CConsole::mCursorMaxX = 0;
uint32_t CConsole::mCursorMaxY = 0;

uint32_t CConsole::mTabSize = 4;


void (*CConsole::mSetPixel)(uint32_t x, uint32_t y, uint32_t color) = NULL;
void (*CConsole::mFill)(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) = NULL;

ReturnValue_t CConsole::Init(const limine_framebuffer_response *pLFBInfo) {
	
	if (pLFBInfo == NULL) {
		return RETVAL_ERROR_NO_FRAMEBUFFER;
	}
	limine_framebuffer *_LimineLFB = pLFBInfo->framebuffers[0];
	if (_LimineLFB == NULL) {
		return RETVAL_ERROR_NO_FRAMEBUFFER;
	}
	
	mFramebuffer = static_cast<void*>(_LimineLFB->address);
	mPitch = _LimineLFB->pitch;
	mWidth = _LimineLFB->width;
	mHeight = _LimineLFB->height;
	mBPP = _LimineLFB->bpp;
	

	mCursorX = 0;
	mCursorY = 0;
	mCursorWidth = 8;
	mCursorHeight = 16;
	mCursorMaxX = mWidth / mCursorWidth;
	mCursorMaxY = mHeight / mCursorHeight;
	
	mTitleHeight = 1;
	switch (mBPP) {
		case 32:
			mSetPixel = &SetPixel32;
			mFill = &Fill32;
			break;
		case 24:
			mSetPixel = &SetPixel24;
			mFill = &Fill24;
			break;
		default:
			mSetPixel = NULL;
			mFill = NULL;
			return RETVAL_ERROR_VIDEOMODE;
	}
		
	return RETVAL_OK;
}
	
uint32_t CConsole::GetWidth(void) {
	return mWidth;
}

uint32_t CConsole::GetHeight(void) {
	return mHeight;
}

uint32_t CConsole::GetBPP(void) {
	return mBPP;
}

size_t CConsole::GetPitch(void) {
	return mPitch;
}

void CConsole::PrintChar(const uint8_t c, uint32_t x, uint32_t y,
	uint32_t fg_color, uint32_t bg_color)
{
	
	x *= mCursorWidth;
	y *= mCursorHeight;
	
	for (uint32_t row = 0; row < 16; row++) {
		for (uint32_t col = 0; col < 8; col++) {
			uint32_t _x = x + col;
			uint32_t _y = y + row;
			if (_x >= mWidth)
				continue;
			if (_y >= mHeight)
				continue;
				
			if (STDFont.a[c].a[row] & (0x80 >> col)) {				
				mSetPixel(_x, _y, fg_color);
			} else {
				mSetPixel(_x, _y, bg_color);
			}
		}
	}
}

void CConsole::PrintCharAlpha(const uint8_t c, uint32_t x, uint32_t y, 
		uint32_t fg_color)
{
	
	x *= mCursorWidth;
	y *= mCursorHeight;
	
	for (uint32_t row = 0; row < 16; row++) {
		for (uint32_t col = 0; col < 8; col++) {
			uint32_t _x = x + col;
			uint32_t _y = y + row;
			if (_x >= mWidth)
				continue;
			if (_y >= mHeight)
				continue;
				
			if (STDFont.a[c].a[row] & (0x80 >> col)) {				
				SetPixel(_x, _y, fg_color);
			}
		}
	}
}

void CConsole::PrintTitle(void) {	
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
	
void CConsole::Clear(void) {	
	
	// Clear all
	Fill(0, 0, mCursorMaxX, mCursorMaxY, mBGColor);
	
	// Background for Title
	Fill(0, 0, mCursorMaxX, mTitleHeight, mTitleBGColor);
	
	// Draw Title
	PrintTitle();

	mCursorX = 0;
	mCursorY = mTitleHeight;

}

void CConsole::SetFGColor(uint32_t color) {
	mFGColor = color;
}

void CConsole::SetBGColor(uint32_t color) {
	mBGColor = color;
}

void CConsole::SetTitleFGColor(uint32_t color) {
	mTitleFGColor = color;
	
	// Background for Title
	Fill(0, 0, mCursorMaxX, mTitleHeight, mTitleBGColor);
	
	// Draw Title
	PrintTitle();
}

void CConsole::SetTitleBGColor(uint32_t color) {
	mTitleBGColor = color;
	
	// Background for Title
	Fill(0, 0, mCursorMaxX, mTitleHeight, mTitleBGColor);
	
	// Draw Title
	PrintTitle();
}

void CConsole::SetTitleText(const char *pText) {
	mTitle = pText;
	
	// Background for Title
	Fill(0, 0, mCursorMaxX, mTitleHeight, mTitleBGColor);
	
	// Draw Title
	PrintTitle();
}

void CConsole::Print(const char *pText) {
	size_t i = 0;
	
	while (pText[i] != 0) {
		
		if (pText[i] == '\n') {
			mCursorX = 0;
			mCursorY += 1;
		} else if (pText[i] == '\r') {
			mCursorX = 0;
		} else if (pText[i] == '\t') {
			mCursorX = (mCursorX + mTabSize) & ~(mTabSize - 1);
		} else {
			PrintChar(pText[i], mCursorX, mCursorY, mFGColor, mBGColor);
			mCursorX += 1;
		}

		
		if (mCursorX >= mCursorMaxX) {
			mCursorX = 0;
			mCursorY += 1;
		}
		if (mCursorY >= mCursorMaxY) {
			ScrollDown(1);
		}
		
		i++;
	}
	
}

void CConsole::ScrollDown(const uint32_t pLines) {
	
	if (pLines == 0)
		return;
	if (pLines >= (mCursorMaxY - mTitleHeight)) {
		Clear();
		return;
	}
	
	uintptr_t _SrcAddress = reinterpret_cast<uintptr_t>(GetFramebufferAddress());
	uintptr_t _DestAddress = reinterpret_cast<uintptr_t>(GetFramebufferAddress());
	size_t _Size = GetFramebufferSize();
	
	
	_SrcAddress += mPitch * (pLines + mTitleHeight) * mCursorHeight;
	_DestAddress += mPitch * mTitleHeight * mCursorHeight;
	_Size -= (_SrcAddress - reinterpret_cast<uintptr_t>(GetFramebufferAddress()));		

	
	memmove(reinterpret_cast<void*>(_DestAddress), reinterpret_cast<void*>(_SrcAddress), _Size);
	
	Fill(0, mCursorMaxY - pLines, mCursorMaxX + 1, mCursorMaxY + 1, mBGColor);
	
	if (pLines >= mCursorY) {
			mCursorY = 1;
	} else {
		mCursorY -= pLines;
	}
	
}

void *CConsole::GetFramebufferAddress(void) {
	return mFramebuffer;
}

size_t CConsole::GetFramebufferSize(void) {
	return mHeight * mPitch;
}

uint32_t CConsole::ConvertColor24(uint32_t color) {	
	return color & 0x00FFFFFF;
}

uint32_t CConsole::ConvertColor32(uint32_t color) {	
	return color;
}

void CConsole::SetPixel24(uint32_t x, uint32_t y, uint32_t color) {
	
	if (x >= mWidth)
		return;
	if (y >= mHeight)
		return;
	
	color = ConvertColor24(color);
	
	volatile uint32_t *target = reinterpret_cast<uint32_t*>((reinterpret_cast<uintptr_t>(GetFramebufferAddress())) + (y * mPitch) + (x * 3));
	uint32_t tmp = *target & 0xFF000000;
	tmp |= color;
	*target = color;
}

void CConsole::SetPixel32(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= mWidth)
		return;
	if (y >= mHeight)
		return;
		
	color = ConvertColor32(color);
	
	volatile uint32_t *target = reinterpret_cast<uint32_t*>((reinterpret_cast<uintptr_t>(GetFramebufferAddress())) + (y * mPitch) + (x * 4));
	*target = color;
}

void CConsole::Fill24(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	
	left *= mCursorWidth;
	top *= mCursorHeight;
	right *= mCursorWidth;
	bottom *= mCursorHeight;
	
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

			volatile uint32_t *target = reinterpret_cast<uint32_t*>((reinterpret_cast<uintptr_t>(GetFramebufferAddress())) + (_y * mPitch) + (_x * 3));
			uint32_t tmp = *target & 0xFF000000;
			tmp |= color;
			*target = color;
		}
	}
}

void CConsole::Fill32(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	
	left *= mCursorWidth;
	top *= mCursorHeight;
	right *= mCursorWidth;
	bottom *= mCursorHeight;
	
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

			volatile uint32_t *target = reinterpret_cast<uint32_t*>((reinterpret_cast<uintptr_t>(GetFramebufferAddress())) + (_y * mPitch) + (_x * 4));
			*target = color;
		}
	}
}

void CConsole::SetPixel(uint32_t x, uint32_t y, uint32_t color) {
	mSetPixel(x, y, color);
}

void CConsole::Fill(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) {
	mFill(left, top, right, bottom, color);
}

uint32_t CConsole::GetTabSize(void) {
	return mTabSize;
}

void CConsole::SetTabSize(uint32_t pTabSize) {
	mTabSize = pTabSize;
}


