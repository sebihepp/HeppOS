
.section .init
.global _init
.type _init, @function

_init:
	pushfq
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	
	pushq %rsi
	pushq %rdi
	pushq %rbp
	
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %r16
	
	
	pushq %rbp
	movq %rsp, %rbp
	
	
.section .fini
.global _fini
.type _fini, @function

_fini:
	pushfq
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	
	pushq %rsi
	pushq %rdi
	pushq %rbp
	
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %r16
	
	pushq %rbp
	movq %rsp, %rbp
