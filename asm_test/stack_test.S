/**
 * Based off of entry_64.S and calling.h in the linux source,
 * and trys to emulate composite's kern_entry.S
 */

//#include <asm/asm-offsets.h> // Need?
//#include <asm/segment.h> // Need?
//#include <asm/thread_info.h> // Need?
//#include <linux/linkage.h> // Need?
//#include "asym_exec_dom.h"  // Need? - is called "deadweight" in it's comments. Only actually used in hijack.c
//#include "../../../../kernel/include/asm_ipc_defs.h"

#include "calling64.h"

	/* Just some code to make sure it all compiles and links */
.text
	.globl main
main:
# Place some values in some soon to be clobbered regs
	mov $1, %r8
	mov $2, %r9

# Save them
	SAVE_ALL

# Clobber them
	xor %r8, %r8
	xor %r9, %r9

# Restore them
	RESTORE_ALL

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
