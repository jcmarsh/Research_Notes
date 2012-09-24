	/* test to see how the jmp command works */
.text
	.globl main
main:
# Welcome message
	mov $1, %rax 		#sys_write
	mov $1, %rdi		#sdout
	mov $welcome, %rsi	#message address
	mov (welcome_l), %rdx		#message length
	syscall
	jmp next

# Should not be printed
	mov $1, %rax
	mov $1, %rdi
	mov $fail, %rsi
	mov (fail_l), %rdx
	syscall

next:
# Should be printed
	mov $1, %rax
	mov $1, %rdi
	mov $print, %rsi
	mov (print_l), %rdx
	syscall

#sys_exit(return_code)
end:
	mov $60, %rax		#sys_exit
	mov $0, %rdi 		#return 0 (success)
	syscall

.data
welcome:
	.ascii "Welcome!\n\0"
welcome_l:
	.quad . -welcome
fail:
	.ascii "Your program is bad, and you should feel bad.\n\0"
fail_l:
	.quad . -fail
print:
	.ascii "Should be printed.\n\0"
print_l:
	.quad . -print
