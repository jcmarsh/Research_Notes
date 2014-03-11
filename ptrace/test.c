
#include "utility.h"

int main(int argc, char** argv) {
  pid_t child_pid = -1;
  int status = -1;
  long reg;
  int ret_value = 0;
  struct user_regs_struct child_regs;
  struct user_regs_struct copy_regs;

  child_pid = fork();

  if (child_pid >=0) { // Successful fork
    if (child_pid == 0) {
      printf("I am the child\n");
      ptrace(PTRACE_TRACEME, 0, NULL, NULL);
      execl("/bin/ls", "ls", NULL);
    } else {
      printf("I am the parent\n");

      while(1) {
	waitpid(child_pid, &status, 0);

	if(WIFEXITED(status) || WIFSIGNALED(status)) {
	  break;
	}

	if (ptrace(PTRACE_GETREGS, child_pid, NULL, &child_regs) < 0) {
	  printf("GETREGS error.\n");
	  return 0;
	}

	//printRegs(&child_regs);
	PRINT_REG(orig_rax, (&child_regs));

	//child_regs.r8 = 0xDEADBEEF;
	//child_regs.orig_rax = 64;
	
	//if (ptrace(PTRACE_SETREGS, child_pid, NULL, &child_regs) < 0) {
	//  printf("SETREGS error.\n");
	//  return 0;
	//}

	//if (ptrace(PTRACE_GETREGS, child_pid, NULL, &child_regs) < 0) {
	//  printf("GETREGS error.\n");
	//  return 0;
	//}

	//printREGS(&child_regs);

	ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL);
      }
    }
  }
  return 0;
}
