#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/wait.h>

void printREGS(struct user_regs_struct * regs) {
  printf("R15: %lX\n", regs->r15);
  printf("R14: %lX\n", regs->r14);
  printf("R13: %lX\n", regs->r13);
  printf("R12: %lX\n", regs->r12);
  printf("RBP: %lX\n", regs->rbp);
  printf("RBX: %lX\n", regs->rbx);
  printf("R11: %lX\n", regs->r11);
  printf("R10: %lX\n", regs->r10);
  printf("R9:  %lX\n", regs->r9);
  printf("R8:  %lX\n", regs->r8);
  printf("RAX: %lX\n", regs->rax);
  printf("RCX: %lX\n", regs->rcx);
  printf("RDX: %lX\n", regs->rdx);
  printf("RSI: %lX\n", regs->rsi);
  printf("RDI: %lX\n", regs->rdi);
  printf("ORIG_RAX: %lX\n", regs->orig_rax);
  printf("RIP: %lX\n", regs->rip);
  printf("CS:  %lX\n", regs->cs);
  printf("EFLAGS: %lX\n", regs->eflags);
  printf("RSP: %lX\n", regs->rsp);
  printf("SS:  %lX\n", regs->ss);
  printf("FS_BASE: %lX\n", regs->fs_base);
  printf("GS_BASE: %lX\n", regs->gs_base);
  printf("DS:  %lX\n", regs->ds);
  printf("ES:  %lX\n", regs->es);
  printf("FS:  %lX\n", regs->fs);
  printf("GS:  %lX\n", regs->gs);
}

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

	printREGS(&child_regs);

	child_regs.r8 = 0xDEADBEEF;
	child_regs.orig_rax = 0x1;

	if (ptrace(PTRACE_SETREGS, child_pid, NULL, &child_regs) < 0) {
	  printf("SETREGS error.\n");
	  return 0;
	}

	if (ptrace(PTRACE_GETREGS, child_pid, NULL, &child_regs) < 0) {
	  printf("GETREGS error.\n");
	  return 0;
	}

	printREGS(&child_regs);

	ptrace(PTRACE_CONT, child_pid, NULL, NULL);
      }
    }
  }
  return 0;
}
