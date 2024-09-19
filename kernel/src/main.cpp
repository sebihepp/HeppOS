
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "retvals.hpp"
#include "multiboot2.hpp"
#include "console.hpp"


extern "C" void kmain(uint32_t magic, multiboot2_info_t *mbi) {

	retval_t retval;
	
	retval = Console::Init(mbi);
	if (retval != RETVAL_OK) {
		return;
	}
	
	Console::SetTitleText("HeppOS");
	Console::Clear();
	
	
	//Loop forever
	asm volatile (
		"cli;\n"
		"1:"
		"hlt;\n"
		"jmp 1b;\n"
	);
	
	return;
}
