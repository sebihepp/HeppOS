
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "retvals.h"
#include "multiboot2.hpp"

 
extern "C" void kmain(uint32_t magic, multiboot2_info_t *mbi) {


	//Loop forever
	asm volatile (
		"cli;\n"
		"1:"
		"hlt;\n"
		"jmp 1b;\n"
	);
	
	return;
}
