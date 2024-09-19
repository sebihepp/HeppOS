

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limine.h>


__attribute__((used, section(".requests"))) static volatile LIMINE_BASE_REVISION(2);
__attribute__((used, section(".requests"))) static volatile limine_framebuffer_request FramebufferRequest = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0,
	.response = NULL
};

__attribute__((used, section(".requests_start_marker"))) static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".requests_end_marker"))) static volatile LIMINE_REQUESTS_END_MARKER;


void kmain(void) {


	while (true) {
		asm volatile ("hlt;\n");
	}
}
