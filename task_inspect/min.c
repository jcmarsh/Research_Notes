/*
 * Author - James Marshall
 */

#include "../include/replicas.h"

#include <linux/kernel.h>
#include <sys/syscall.h>
#define __NR_inspect 312

#define REP_COUNT 2

long inspect_syscall(void) {
  return syscall(__NR_inspect);
}

void syscallHandler(int signo) {
  long int retval = inspect_syscall();
  printf("Hopefully called the syscall. retval = %ld\n", retval);
  return;
}

// Replica related data
struct replica replicas[REP_COUNT];

char* controller_name;
// pipes to external components (not replicas)
int pipe_count = 0;
struct typed_pipe ext_pipes[PIPE_LIMIT];

int main(int argc, const char **argv) {
  if (signal(SIGUSR2, syscallHandler) == SIG_ERR) {
    perror("Failed to register the invoke syscall handler");
    return -1;
  }

  controller_name = (char*)(argv[1]);

  initReplicas(replicas, REP_COUNT, controller_name, 10);
  createPipes(replicas, REP_COUNT, NULL, 0);
  forkReplicas(replicas, REP_COUNT);

  while(1) {
    sleep(1);
  }

  return 0;
}
