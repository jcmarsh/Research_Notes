#Woo! Comments. "Hello World"

.text
	.globl main
	#sys_write(stdout, message, length)
main:
	mov $1, %rax 		#sys_write
	mov $1, %rdi		#sdout
	mov $message, %rsi	#message address
	mov $12, %rdx		#message length
	syscall

	#sys_exit(return_code)

	mov $60, %rax		#sys_exit
	mov $0, %rdi 		#return 0 (success)
	syscall

.data
message:
	.string "Hello James\n"
