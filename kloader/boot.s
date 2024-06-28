

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
	.long 1280											# Width
	.long 720											# Heigth
	.long 32											# Depth
	.long 0												# Padding for 8 Byte Alignment
	
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
	
	# Setup stack
	movl $stack_top, %esp
	
	# Save Multiboot2 tags and magic
	pushl %ebx		# Tags
	pushl %eax		# Magic
	
	# Check maximum CPUID
	movl $0x0, %eax
	cpuid
	movl %eax, MaxCPUID

	# Check maximum Exteded CPUID
	movl $0x80000000, %eax
	cpuid
	movl %eax, MaxExtCPUID
	cmpl $0x80000001, %eax
	jae 3f
	movl $2, %eax
	jmp _error
3:
	
	
	# Check for 64Bit Capable
	movl $0x80000001, %eax
	cpuid
	test $0x20000000, %edx
	jnz 4f
	movl $3, %eax
	jmp _error
4:

	# continue with loading
	call main

	
_error:
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

MaxCPUID:
	.long 0
MaxExtCPUID:
	.long 0
	