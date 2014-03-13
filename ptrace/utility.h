#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/wait.h>

#define PRINT_REG(NAME, REG) printf("%s\t: %lX\t%lu\n", #NAME, REG->NAME, REG->NAME)

#if __WORDSIZE == 64

void printRegs(struct user_regs_struct * regs) {
  PRINT_REG(r15, regs);
  PRINT_REG(r14, regs);
  PRINT_REG(r13, regs);
  PRINT_REG(r12, regs);
  PRINT_REG(rbp, regs);
  PRINT_REG(rbx, regs);
  PRINT_REG(r11, regs);
  PRINT_REG(r10, regs);
  PRINT_REG(r9, regs);
  PRINT_REG(r8, regs);
  PRINT_REG(rax, regs);
  PRINT_REG(rcx, regs);
  PRINT_REG(rdx, regs);
  PRINT_REG(rsi, regs);
  PRINT_REG(rdi, regs);
  PRINT_REG(orig_rax, regs);
  PRINT_REG(rip, regs);
  PRINT_REG(cs, regs);
  PRINT_REG(eflags, regs);
  PRINT_REG(rsp, regs);
  PRINT_REG(ss, regs);
  PRINT_REG(fs_base, regs);
  PRINT_REG(gs_base, regs);
  PRINT_REG(ds, regs);
  PRINT_REG(es, regs);
  PRINT_REG(fs, regs);
  PRINT_REG(gs, regs);
}

#else // 32 bit wordsize... x86

void printRegs(struct user_regs_struct * regs) {
  PRINT_REG(ebx, regs);
  PRINT_REG(ecx, regs);
  PRINT_REG(edx, regs);
  PRINT_REG(esi, regs);
  PRINT_REG(edi, regs);
  PRINT_REG(ebp, regs);
  PRINT_REG(eax, regs);
  PRINT_REG(xds, regs);
  PRINT_REG(xes, regs);
  PRINT_REG(xfs, regs);
  PRINT_REG(xgs, regs);
  PRINT_REG(orig_eax, regs);
  PRINT_REG(eip, regs);
  PRINT_REG(xcs, regs);
  PRINT_REG(eflags, regs);
  PRINT_REG(esp, regs);
  PRINT_REG(xss, regs);
}

#endif // __WORDSIZE


