
.section .init
.global _init
.type _init, @function

_init:
	pushl %ebp
	movl %esp, %ebp
	pushal
	pushfl
	
	
.section .fini
.global _fini
.type _fini, @function

_fini:
	pushl %ebp
	movl %esp, %ebp
	pushal
	pushfl
	