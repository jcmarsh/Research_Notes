/*
 * 
 * James Marshall
 */

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/taslimited.h"

// For the syscall
#include <linux/kernel.h>
#include <sys/syscall.h>
#define __NR_inspect 312

void enterLoop();
void command();

void restartHandler(int signo) {
  // fork
  timestamp_t last = generate_timestamp();
  pid_t currentPID = fork();

  if (currentPID >= 0) { // Successful fork
    if (currentPID == 0) { // Child process
      timestamp_t current = generate_timestamp();
      printf("%lld\n", current - last);

      // unblock the signal
      sigset_t signal_set;
      sigemptyset(&signal_set);
      sigaddset(&signal_set, SIGUSR1);
      sigaddset(&signal_set, SIGUSR2); // syscall
      sigprocmask(SIG_UNBLOCK, &signal_set, NULL);

      EveryTAS();

      return;
    } else {   // Parent just returns
      waitpid(-1, NULL, WNOHANG);
      return;
    }
  } else {
    printf("Fork error!\n");
    return;
  }
}

long inspect_syscall(void) {
  return syscall(__NR_inspect);
}

void syscallHandler(int signo) {
  long int retval = inspect_syscall();
  printf("Hopefully called the syscall. retval = %ld\n", retval);
  return;
}

int main(int argc, const char **argv) {
  if (signal(SIGUSR1, restartHandler) == SIG_ERR) {
    perror("Failed to register the restart handler");
    return -1;
  }

  if (signal(SIGUSR2, syscallHandler) == SIG_ERR) {
    perror("Failed to register the invoke syscall handler");
    return -1;
  }

  while(1) {
    sleep(1);
  }

  return 0;
}
