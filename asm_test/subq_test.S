	/* Try out subq for adjusting the stack pointer */
.text
	.globl main
main:
	subq $8, %rsp
	subq $15*8, %rsp
#	subq $6*8, %rsp

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
fail:
	.ascii "Your program is bad, and you should feel bad.\n\0"
fail_l:
	.quad . -fail
