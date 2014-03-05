#include <unistd.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <signal.h>
#include <sys/user.h>

#define CHILD_NUM 3

const int starting_n = 42;
const char *reg_names[27];

// Can likely get rid of by using GETREGS for ptrace (and using sys/user.h).
void init_reg_names() {
  reg_names[0] = "R15";
  reg_names[1] = "R14";
  reg_names[2] = "R13";
  reg_names[3] = "R12";
  reg_names[4] = "RBP";
  reg_names[5] = "RBX";
  reg_names[6] = "R11";
  reg_names[7] = "R10";
  reg_names[8] = "R9";
  reg_names[9] = "R8";
  reg_names[10] = "RAX";
  reg_names[11] = "RCX";
  reg_names[12] = "RDX";
  reg_names[13] = "RSI";
  reg_names[14] = "RDI";
  reg_names[15] = "ORIG_RAX";
  reg_names[16] = "RIP";
  reg_names[17] = "CS";
  reg_names[18] = "EFLAGS";
  reg_names[19] = "RSP";
  reg_names[20] = "SS";
  reg_names[21] = "FS_BASE";
  reg_names[22] = "GS_BASE";
  reg_names[23] = "DS";
  reg_names[24] = "ES";
  reg_names[25] = "FS";
  reg_names[26] = "GS";
}

void allregs(pid) {
  long reg;
  int index;

  for (index = 0; index < 27; index++) {
    reg = ptrace(PTRACE_PEEKUSER, pid, 8 * index, NULL);
    printf("Reg: %s = %ld\n", reg_names[index], reg);
  }
}

// Find the nth fibonacci number
unsigned long fib(int n) {
  if (n == 1 || n == 2) {
    return 1;
  } else {
    return fib(n-1) + fib(n-2);
  }
}

int main(int argc, char** argv) {
  pid_t childPIDs[CHILD_NUM]; pid_t currentPID = 0;
  int isChild = 0;
  int index = 0;
  long reg;

  init_reg_names();

  // Fork three children
  for (index = 0; index < CHILD_NUM; index++) {
    currentPID = fork();
    
    if (currentPID >= 0) { // Successful fork
      if (currentPID == 0) { // Child process
	isChild = 1;
	break;
      } else { // Parent Process
	childPIDs[index] = currentPID;
      }
    }
  }

  if (isChild) {
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    raise(SIGCONT);
    printf("%dth fibonacci number is: %lu\n", starting_n, fib(starting_n));
  } else {
    printf("I AM THE OVERLORD\n");
    wait(NULL);
    allregs(childPIDs[0]);
    ptrace(PTRACE_CONT, childPIDs[0], NULL, NULL);
    wait(NULL);
    printf("Oh, hello.\n");
    wait(NULL);
    wait(NULL);
  }

  //    if (childPID == 0) {
  //    printf("I am the child\n");
  //    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
  //    execl("/bin/ls", "ls", NULL);
  //  } else {
  //    printf("I am the parent\n");
  //    currentPID = wait(NULL);
  //    //     reg = ptrace(PTRACE_PEEKUSER, childPID, 8 * ORIG_RAX, NULL);
  //    allregs(currentPID);
  //    //printf("Whoa son, %ld syscall\n", reg);
  //    ptrace(PTRACE_CONT, childPID, NULL, NULL);
  //  }
  //}
  return 0;
}
