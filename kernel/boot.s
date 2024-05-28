

.include "multiboot2.s"

.set STACK_SIZE,		(16*1024) # Stack of 16 KByte

.section ".multiboot"
.align 8
_multiboot_header:
	.long MULTIBOOT2_HEADER_MAGIC						# Multiboot magic number
	.long MULTIBOOT2_ARCHITECTURE_I386					# Architecture (0 = i386)
	.long _multiboot_header - _multiboot_header_end		# Size of Headers
	.long -MULTIBOOT2_HEADER_MAGIC - MULTIBOOT2_ARCHITECTURE_I386 - (_multiboot_header - _multiboot_header_end)		# Checksum 

	.short MULTIBOOT2_HEADER_TAG_MODULE_ALIGN			# Type 6 = Modules must be page aligned
	.short MULTIBOOT2_HEADER_TAG_MANDATORY				# Flags
	.long 8												# Size
	
	.short MULTIBOOT2_HEADER_TAG_END					# Type 0 = end of tags
	.short MULTIBOOT2_HEADER_TAG_MANDATORY				# Flags
	.long 8												# Size
_multiboot_header_end:


.global _start
.type _start, @function

.text
_start:
	cli	
	mov $stack_top, %esp

	push %ebx
	push %eax
	call kmain

	pop %eax
	pop %ebx

# Print reboot message
_print_reboot_message:
	push %eax
	push $reboot_message
	call kprint
	pop %eax
	pop %eax
	
# Loop forever so that the user can shutdown or restart the computer	
	cli
_1:
	hlt
	jmp _1


.section ".bss"
	.align 4096
	.lcomm stack_bottom, STACK_SIZE
stack_top:

.data

reboot_message: .string "\nYou can now turn off the computer...\n"
