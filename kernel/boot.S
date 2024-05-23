

.section ".multiboot"
.align 8

_mb_start:
	.long 0xE85250D6	# Multiboot magic number
	.long 0x0		# Architecture (0 = i386)
	.long _mb_end - _mb_start	# Size of Headers
	.long -0xE85250D6 - 0x0 - (_mb_end - _mb_start)		# Checksum 

	.short 0		# Type 0 = end of tags
	.short 0		# Flags
	.long 8			# Size
_mb_end:

.global _start
.type _start, @function

.section ".text"
_start:
	cli	
	mov $stack_top, %esp

	push %eax
	push %ebx
	call kmain

	pop %ebx
	pop %eax


	
__hlt_loop:
	hlt
	jmp __hlt_loop

.section ".bss"
stack_bottom:
	.skip (1024*16)
stack_top:

