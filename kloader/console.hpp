
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
	
public:	
	static retval_t Init(const multiboot2_info_t *mbi);
	
	static void PrintChar(uint8_t c, uint32_t x, uint32_t y,
		uint32_t fg_color, uint32_t bg_color);
	static void PrintCharAlpha(uint8_t c, uint32_t x, uint32_t y, 
		uint32_t fg_color);
	
	static void Clear(uint32_t color = 0x00000000);
	
	static void Fill(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, uint32_t color);
	
	static uint32_t GetWidth(void);
	static uint32_t GetHeight(void);
	
};

#endif
