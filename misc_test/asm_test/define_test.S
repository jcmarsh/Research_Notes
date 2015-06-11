	/* The defines aren't working, so trying with them inline */
.text
	.globl main
main:
# Place some values in some soon to be clobbered regs
	mov $1, %r8
	mov $2, %r9

# Save them
	subq	$9*8, %rsp
	movq	%rdi, 8*8(%rsp)
	movq	%rsi, 7*8(%rsp)
	movq	%rdx, 6*8(%rsp)
	movq	%rcx, 5*8(%rsp)
	movq	%rax, 4*8(%rsp)
	movq	%r8, 3*8(%rsp)
	movq	%r9, 2*8(%rsp)
	movq	%r10, 1*8(%rsp)
	movq	%r11, (%rsp)

	subq 	$6*8, %rsp
	movq 	%rbx, 5*8(%rsp)
	movq 	%rbp, 4*8(%rsp)
	movq 	%r12, 3*8(%rsp)
	movq 	%r13, 2*8(%rsp)
	movq 	%r14, 1*8(%rsp)
	movq 	%r15, (%rsp)

# Clobber them
	xor %r8, %r8
	xor %r9, %r9

# Restore them
	movq	(%rsp), %r15
	movq	1*8(%rsp), %r14
	movq	2*8(%rsp), %r13
	movq	3*8(%rsp), %r12
	movq	4*8(%rsp), %rbp
	movq	5*8(%rsp), %rbx
	addq	$6*8, %rsp

	movq	(%rsp), %r11
	movq	1*8(%rsp), %r10
	movq	2*8(%rsp), %r9
	movq	3*8(%rsp), %r8
	movq	4*8(%rsp), %rax
	movq	5*8(%rsp), %rcx
	movq	6*8(%rsp), %rdx
	movq	7*8(%rsp), %rsi
	movq	8*8(%rsp), %rdi
	addq	$9*8, %rsp

# Check and print results
	cmp $1, %r8
	jne error

	cmp $2, %r9
	jne error

# Success
	mov $1, %rax 		#sys_write
	mov $1, %rdi		#sdout
	mov $success, %rsi	#message address
	mov (success_l), %rdx		#message length
	syscall
	jmp end

# Failure
error:
	mov $1, %rax
	mov $1, %rdi
	mov $fail, %rsi
	mov (fail_l), %rdx
	syscall

#sys_exit(return_code)
end:
	mov $60, %rax		#sys_exit
	mov $0, %rdi 		#return 0 (success)
	syscall

.data
success:
	.ascii "Huzzah, it worked!\n\0"
success_l:
	.quad . -success  # The . means "here"
fail:
	.ascii "Your program is bad, and you should feel bad.\n\0"
fail_l:
	.quad . -fail
