#include "calling64.h"


.text
	.globl main
main:
	movq    $0xdeadbeef, %rax
	pushq   %rax
	popq    %rbx
	cmp	%rax, %rbx
	jne    	error
	//call  *(test_function)

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
