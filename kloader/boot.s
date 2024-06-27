

.include "multiboot2_defines.s"

.set STACK_SIZE,		(16*1024) # Stack of 16 KByte

.section ".multiboot"
.align 8
_multiboot_header:
	.long MULTIBOOT2_HEADER_MAGIC						# Multiboot magic number
	.long MULTIBOOT2_ARCHITECTURE_I386					# Architecture (0 = i386)
	.long _multiboot_header - _multiboot_header_end		# Size of Headers
	.long -MULTIBOOT2_HEADER_MAGIC - MULTIBOOT2_ARCHITECTURE_I386 - (_multiboot_header - _multiboot_header_end)		# Checksum 
	
	.short MULTIBOOT2_HEADER_TAG_FRAMEBUFFER			# Type 5 = Framebuffer request
	.short MULTIBOOT2_HEADER_TAG_OPTIONAL				# Flags
	.long 20											# Size
	.long 1024											# Width
	.long 768											# Heigth
	.long 32											# Depth
	
	.short MULTIBOOT2_HEADER_TAG_MODULE_ALIGN			# Type 6 = Modules must be page aligned
	.short MULTIBOOT2_HEADER_TAG_MANDATORY				# Flags
	.long 8												# Size

	.short MULTIBOOT2_HEADER_TAG_END					# Type 0 = end of tags
	.short MULTIBOOT2_HEADER_TAG_MANDATORY				# Flags
	.long 8												# Size
_multiboot_header_end:


.global _start
.type _start, @function

.section ".text"
_start:
	cli	
	mov $stack_top, %esp
	
# Loop forever so that the user can shutdown or restart the computer	
	cli
1:
	hlt
	jmp 1b


.section ".bss"
	.align 4096
	.lcomm stack_bottom, STACK_SIZE
stack_top:


.section ".data"

