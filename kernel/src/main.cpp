

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void kmain(void) {


	while (true) {
		asm volatile ("hlt;\n");
	}
}
