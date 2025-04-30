
.macro interrupt_save
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	
	pushq %rsp
	pushq %rbp
	pushq %rsi
	pushq %rdi
	
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11

	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	
	xorq %rax, %rax
	movw %ds, %ax
	pushq %rax
	movw %es, %ax
	pushq %rax
	pushq %fs
	pushq %gs
.endm

.macro interrupt_restore	
	popq %gs
	popq %fs
	popq %rax
	movw %ax, %es
	popq %rax
	movw %ax, %ds
	
	popq %r15
	popq %r14
	popq %r13
	popq %r12

	popq %r11
	popq %r10
	popq %r9
	popq %r8
	
	popq %rdi
	popq %rsi
	popq %rbp
	popq %rsp
	
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
.endm


.macro isr_wrapper_stub number:req
.global isr_wrapper_\number
.type isr_wrapper_\number , @function
isr_wrapper_\number :
	
	pushq $0
	
	interrupt_save
	
	movq $\number , %rdi
	movq %rsp, %rsi
	
	cld
	call ISRHandler
	
	interrupt_restore
	
	addq $8, %rsp
	iretq
.endm


.macro exception_wrapper_stub number:req
.global exception_wrapper_\number
.type exception_wrapper_error_\number , @function
exception_wrapper_\number :
	
	pushq $0
	
	interrupt_save
	
	movq $\number , %rdi
	movq %rsp, %rsi
	
	cld
	call ExceptionHandler
	
	interrupt_restore
	
	addq $8, %rsp
	iretq
.endm


	
.macro exception_wrapper_error_stub number:req
.global exception_wrapper_error_\number
.type exception_wrapper_error_\number , @function
exception_wrapper_error_\number :

	interrupt_save
	
	movq $\number , %rdi
	movq %rsp, %rsi
	
	cld
	call ExceptionHandler
	
	interrupt_restore
	
	addq $8, %rsp
	iretq
.endm

.macro isr_wrapper_label number:req
	.quad isr_wrapper_\number
.endm

.section ".text"

exception_wrapper_stub 0
exception_wrapper_stub 1
exception_wrapper_stub 2
exception_wrapper_stub 3
exception_wrapper_stub 4
exception_wrapper_stub 5
exception_wrapper_stub 6
exception_wrapper_stub 7
exception_wrapper_error_stub 8
exception_wrapper_stub 9
exception_wrapper_error_stub 10
exception_wrapper_error_stub 11
exception_wrapper_error_stub 12
exception_wrapper_error_stub 13
exception_wrapper_error_stub 14
exception_wrapper_stub 15
exception_wrapper_stub 16
exception_wrapper_error_stub 17
exception_wrapper_stub 18
exception_wrapper_stub 19
exception_wrapper_stub 20
exception_wrapper_stub 21
exception_wrapper_stub 22
exception_wrapper_stub 23
exception_wrapper_stub 24
exception_wrapper_stub 25
exception_wrapper_stub 26
exception_wrapper_stub 27
exception_wrapper_stub 28
exception_wrapper_stub 29
exception_wrapper_error_stub 30
exception_wrapper_stub 31

.altmacro
.set i,32
.rept 256-32
	isr_wrapper_stub %i
	.set i, i+1
.endr




.section ".data"

.global gISRHandlerAddressTable

gISRHandlerAddressTable:
	.quad exception_wrapper_0 
	.quad exception_wrapper_1 
	.quad exception_wrapper_2
	.quad exception_wrapper_3 
	.quad exception_wrapper_4 
	.quad exception_wrapper_5 
	.quad exception_wrapper_6 
	.quad exception_wrapper_7
	.quad exception_wrapper_error_8
	.quad exception_wrapper_9
	.quad exception_wrapper_error_10
	.quad exception_wrapper_error_11
	.quad exception_wrapper_error_12
	.quad exception_wrapper_error_13
	.quad exception_wrapper_error_14
	.quad exception_wrapper_15
	.quad exception_wrapper_16
	.quad exception_wrapper_error_17
	.quad exception_wrapper_18
	.quad exception_wrapper_19
	.quad exception_wrapper_20
	.quad exception_wrapper_21
	.quad exception_wrapper_22
	.quad exception_wrapper_23
	.quad exception_wrapper_24
	.quad exception_wrapper_25
	.quad exception_wrapper_26
	.quad exception_wrapper_27
	.quad exception_wrapper_28
	.quad exception_wrapper_29
	.quad exception_wrapper_error_30
	.quad exception_wrapper_31
	
.altmacro
.set i,32
.rept 256-32
	isr_wrapper_label %i
	.set i, i+1
.endr
	
