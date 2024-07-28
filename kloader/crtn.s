
.section .init

	popfl
	popal
	popl %ebp
	ret
	
	
.section .fini

	popfl
	popal
	popl %ebp
	ret
	