
#ifndef CONSOLE_HEADER
#define CONSOLE_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <limine.h>
#include <retvals.h>

struct video_char_t {
	uint8_t a[16];
} __attribute__((packed));

struct video_font_t {
	video_char_t a[256];
} __attribute__((packed));


class Console {

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
	
	static void PrintChar(const uint8_t c, uint32_t x, uint32_t y, uint32_t fgcolor, uint32_t bgcolor);
	static void PrintCharAlpha(const uint8_t c, uint32_t x, uint32_t y, uint32_t fgcolor);
	static void PrintTitle(void);

	static uint32_t ConvertColor24(uint32_t color);
	static uint32_t ConvertColor32(uint32_t color);

	static void SetPixel24(uint32_t x, uint32_t y, uint32_t color);
	static void SetPixel32(uint32_t x, uint32_t y, uint32_t color);
	
	static void Fill24(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	static void Fill32(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	
	static void (*mSetPixel)(uint32_t x, uint32_t y, uint32_t color);
	static void (*mFill)(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	
	
	Console();
	~Console();
	
public:	
	static retval_t Init(const limine_framebuffer_response *pLFBInfo);

	static void SetPixel(uint32_t x, uint32_t y, uint32_t color);
	static void Fill(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	
	static void SetFGColor(uint32_t color);
	static void SetBGColor(uint32_t color);

	static void SetTitleFGColor(uint32_t color);
	static void SetTitleBGColor(uint32_t color);	
	static void SetTitleText(const char *pText);
	
	static void Print(const char *pText);
	static void ScrollDown(const uint32_t pLines);
	static void Clear(void);
	
	static void *GetFramebufferAddress(void);
	static size_t GetFramebufferSize(void);
	
	static uint32_t GetWidth(void);
	static uint32_t GetHeight(void);
	static uint32_t GetBPP(void);
	static size_t GetPitch(void);
	
};

#endif