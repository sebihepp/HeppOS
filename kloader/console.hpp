
#ifndef CONSOLE_HEADER
#define CONSOLE_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "multiboot2.hpp"
#include "retvals.hpp"

struct video_char_t {
	uint8_t a[16];
} __attribute__((packed));

struct video_font_t {
	video_char_t a[256];
} __attribute__((packed));


class Console {

private:
	
	static void *Framebuffer;
	static uint32_t Pitch;
	static uint32_t Width;
	static uint32_t Height;
	static uint8_t BPP;

	static uint32_t CursorX;
	static uint32_t CursorY;
	
	static uint32_t FGColor;
	static uint32_t BGColor;
	static uint32_t TitleFGColor;
	static uint32_t TitleBGColor;

	static const uint32_t TitleHeight;
	
	static const char *Title;
	
	static void PrintChar(const uint8_t c, uint32_t x, uint32_t y, uint32_t fgcolor, uint32_t bgcolor);
	static void PrintCharAlpha(const uint8_t c, uint32_t x, uint32_t y, uint32_t fgcolor);
	static void PrintTitle(void);

	static uint8_t ConvertColor8(uint32_t color);
	static uint16_t ConvertColor15(uint32_t color);
	static uint16_t ConvertColor16(uint32_t color);
	static uint32_t ConvertColor24(uint32_t color);
	static uint32_t ConvertColor32(uint32_t color);
	
	static void SetPixel8(uint32_t x, uint32_t y, uint32_t color);
	static void SetPixel15(uint32_t x, uint32_t y, uint32_t color);
	static void SetPixel16(uint32_t x, uint32_t y, uint32_t color);
	static void SetPixel24(uint32_t x, uint32_t y, uint32_t color);
	static void SetPixel32(uint32_t x, uint32_t y, uint32_t color);
	
	static void Fill8(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	static void Fill15(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	static void Fill16(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	static void Fill24(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	static void Fill32(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	
	static void (*SetPixel)(uint32_t x, uint32_t y, uint32_t color);
	static void (*Fill)(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	
	Console();
	~Console();
	
public:	
	static retval_t Init(const multiboot2_info_t *mbi);
	
	static void SetFGColor(uint32_t color);
	static void SetBGColor(uint32_t color);

	static void SetTitleFGColor(uint32_t color);
	static void SetTitleBGColor(uint32_t color);	
	static void SetTitleText(const char *text);
	
	static void Print(const char *text);
	
	static void Clear(void);
	
	static void *GetFramebufferAddress(void);
	static size_t GetFramebufferSize(void);
	
};

#endif
