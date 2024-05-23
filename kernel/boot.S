

.section ".multiboot"

	.long 0xE85250D6	# Multiboot magic number
	.long 0x0		# Architecture (0 = i386)
	.long 0x16		# Size of Headers
	.long -0xE85250D6 - 0x0 - 0x16		# Checksum 


.global _start
.section ".text"

_start:
	cli
__hlt_loop:
	hlt
	jmp __hlt_loop

