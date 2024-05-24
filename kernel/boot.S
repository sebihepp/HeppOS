

.section ".multiboot"
.align 8

_mb_start:
	.long 0xE85250D6	# Multiboot magic number
	.long 0x0		# Architecture (0 = i386)
	.long _mb_end - _mb_start	# Size of Headers
	.long -0xE85250D6 - 0x0 - (_mb_end - _mb_start)		# Checksum 

	.short 6		# Type 6 = Modules must be page aligned
	.short 0		# Flags
	.long 8			# Size
	
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

	push %ebx
	push %eax
	call kmain

	pop %eax
	pop %ebx

# Print reboot message
print_reboot_message:
	push %ecx
	push $reboot_message
	call kprint
	pop %ecx
	pop %ecx
	
# Loop forever so that the user can shutdown or restart the computer	
	cli
_1:
	hlt
	jmp _1


.section ".bss"
stack_bottom:
	.skip (1024*16)
stack_top:

.section ".data"

reboot_message: .string "\nYou can now turn off the computer...\n"
