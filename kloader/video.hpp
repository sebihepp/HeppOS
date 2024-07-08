
#ifndef VIDEO_HEADER
#define VIDEO_HEADER

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


class Video {

private:
	static void *framebuffer;
	static uint32_t pitch;
	static uint32_t width;
	static uint32_t height;
	
public:	
	static retval_t init(multiboot2_info_t *mbi);
	
	static void print_char(uint8_t c, uint32_t x, uint32_t y);
	static void clear(uint32_t color = 0x00000000);
	
	static uint32_t getWidth(void);
	static uint32_t getHeight(void);
	
};

#endif
