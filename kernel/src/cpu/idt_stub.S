
.global isr_wrapper
.type isr_wrapper, @function

.global isr_wrapper_error
.type isr_wrapper_error, @function

.section ".text"
isr_wrapper:
	
	pushq %rax
	pushq %rdi
	pushq %rsi
	pushq %rdx
	pushq %rcx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	
	pushq %rsp
	sub $16, %rsp
	andq $~15, %rsp
	cld
	call isr_handler
	popq %rsp
	
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rcx
	popq %rdx
	popq %rsi
	popq %rdi
	popq %rax
	
	iretq
	
isr_wrapper_error:

	pushq %rax
	pushq %rdi
	pushq %rsi
	pushq %rdx
	pushq %rcx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	
	pushq %rsp
	sub $16, %rsp
	andq $~15, %rsp
	cld
	call isr_handler
	popq %rsp
	
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rcx
	popq %rdx
	popq %rsi
	popq %rdi
	popq %rax

	iretq
	