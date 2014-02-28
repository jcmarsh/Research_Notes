#include <unistd.h>
#include <stdio.h>

#include <sys/ptrace.h>

#include <sys/reg.h>

const char *reg_names[27];

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

int main(int argc, char** argv) {
  pid_t childPID;
  long reg;

  childPID = fork();

  init_reg_names();

  if (childPID >=0) { // Successful fork
    if (childPID == 0) {
      printf("I am the child\n");
      ptrace(PTRACE_TRACEME, 0, NULL, NULL);
      execl("/bin/ls", "ls", NULL);
    } else {
      printf("I am the parent\n");
      wait(NULL);
      //     reg = ptrace(PTRACE_PEEKUSER, childPID, 8 * ORIG_RAX, NULL);
      allregs(childPID);
      //printf("Whoa son, %ld syscall\n", reg);
      ptrace(PTRACE_CONT, childPID, NULL, NULL);
    }
  }
  return 0;
}
