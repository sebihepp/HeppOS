
#ifndef VIDEO_HEADER
#define VIDEO_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "multiboot2.hpp"


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
	
public:	
	static uint32_t init(multiboot2_info_t *mbi);
	
	static void print_char(uint8_t c, uint32_t x, uint32_t y);
	
};

#endif