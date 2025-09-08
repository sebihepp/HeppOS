/* 
    Copyright (C) 2025  Sebastian Hepp aka sebihepp

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#pragma once

#include <ktype.h>

#include <liminestub.h>

struct VideoChar_t {
	uint8_t a[16];
} __attribute__(( packed, aligned(16) ));

struct VideoFont_t {
	VideoChar_t a[256];
} __attribute__((packed));


class CConsole {

private:
	
	static void *mFramebuffer;
	static uint32_t mPitch;
	static uint32_t mWidth;
	static uint32_t mHeight;
	static uint8_t mBPP;
	
	static uint32_t mCursorX;
	static uint32_t mCursorY;
	static uint32_t mCursorWidth;
	static uint32_t mCursorHeight;
	static uint32_t mCursorMaxX;
	static uint32_t mCursorMaxY;

	
	static uint32_t mFGColor;
	static uint32_t mBGColor;
	static uint32_t mTitleFGColor;
	static uint32_t mTitleBGColor;

	static uint32_t mTitleHeight;
	
	static const char *mTitle;
	
	static uint32_t mTabSize;
	
	static void PrintChar(const uint8_t c, uint32_t x, uint32_t y, uint32_t fgcolor, uint32_t bgcolor) __attribute__ (( nothrow ));
	static void PrintCharAlpha(const uint8_t c, uint32_t x, uint32_t y, uint32_t fgcolor) __attribute__ (( nothrow ));
	static void PrintTitle(void) __attribute__ (( nothrow ));

	static uint32_t ConvertColor24(uint32_t color) __attribute__ (( const, nothrow ));
	static uint32_t ConvertColor32(uint32_t color) __attribute__ (( const, nothrow ));

	static void SetPixel24(uint32_t x, uint32_t y, uint32_t color) __attribute__ (( nothrow ));
	static void SetPixel32(uint32_t x, uint32_t y, uint32_t color) __attribute__ (( nothrow ));
	
	static void Fill24(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) __attribute__ (( nothrow ));
	static void Fill32(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) __attribute__ (( nothrow ));
	
	static void (*mSetPixel)(uint32_t x, uint32_t y, uint32_t color);
	static void (*mFill)(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	
	
	CConsole() = delete;
	~CConsole() = delete;
	
public:	
	static ReturnValue_t Init(const limine_framebuffer_response *pLFBInfo) __attribute__ (( nothrow ));

	static void SetPixel(uint32_t x, uint32_t y, uint32_t color) __attribute__ (( nothrow ));
	static void Fill(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color) __attribute__ (( nothrow ));
	
	static void SetFGColor(uint32_t color) __attribute__ (( nothrow ));
	static void SetBGColor(uint32_t color) __attribute__ (( nothrow ));

	static void SetTitleFGColor(uint32_t color) __attribute__ (( nothrow ));
	static void SetTitleBGColor(uint32_t color) __attribute__ (( nothrow ));	
	static void SetTitleText(const char *pText) __attribute__ (( nothrow ));
	
	static void Print(const char *pText) __attribute__ (( nothrow ));
	static void ScrollDown(const uint32_t pLines) __attribute__ (( nothrow ));
	static void Clear(void) __attribute__ (( nothrow ));
	
	static void *GetFramebufferAddress(void) __attribute__ (( nothrow ));
	static size_t GetFramebufferSize(void) __attribute__ (( nothrow ));
	
	static uint32_t GetWidth(void) __attribute__ (( nothrow ));
	static uint32_t GetHeight(void) __attribute__ (( nothrow ));
	static uint32_t GetBPP(void) __attribute__ (( nothrow ));
	static size_t GetPitch(void) __attribute__ (( nothrow ));
	
	static uint32_t GetTabSize(void) __attribute__ (( nothrow ));
	static void SetTabSize(uint32_t pTabSize) __attribute__ (( nothrow ));
	
};
