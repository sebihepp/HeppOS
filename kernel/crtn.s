
.section .init

	popq %r16
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	
	popq %rbp
	popq %rdi
	popq %rsi
	
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax

	popfq
	popq %rbp
	retq
	
	
.section .fini

	popq %r16
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	
	popq %rbp
	popq %rdi
	popq %rsi
	
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	
	popfq
	popq %rbp
	retq
	