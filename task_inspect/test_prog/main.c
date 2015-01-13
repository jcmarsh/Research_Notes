#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

#define __NR_inspect 312

long inspect_syscall(void) {
  return syscall(__NR_inspect);
}

int main(int argc, char* argv[]) {
  long int retval = inspect_syscall();
  printf("Probably just called the syscall. But who knows? retval = %ld\n", retval);
  return 0;
}
