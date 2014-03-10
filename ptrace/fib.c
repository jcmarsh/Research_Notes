#include "utility.h"

#define CHILD_NUM 3

 // Represents one redundant execution, implemented as a thread
struct replica {
  pid_t pid; // The pid of the thread
  int priority; // Not yet implemented
  int pipefd[2]; // pipe to communicate with controller
  // Possibly put a pointer to entry function
};
  

const int starting_n = 42;

// Find the nth fibonacci number
unsigned long fib(int n) {
  if (n == 1 || n == 2) {
    return 1;
  } else {
    return fib(n-1) + fib(n-2);
  }
}

int main(int argc, char** argv) {
  pid_t currentPID = 0;
  struct replica replicas[3];
  int status = -1;
  int finished_count = 0;
  int write_out_pipe = 0;
  int isChild = 0;
  int index = 0;
  unsigned long result = 0;
  char buffer[100] = {0};
  struct user_regs_struct child_regs;

  // Init three replicas
  for (index = 0; index < CHILD_NUM; index++) {
    if (pipe(replicas[index].pipefd) == -1) {
      printf("Pipe error!\n");
      return 0;
    }
    replicas[index].pid = -1;
    replicas[index].priority = -1;
  }
    
    
  // Fork three children
  for (index = 0; index < CHILD_NUM; index++) {
    currentPID = fork();
    
    if (currentPID >= 0) { // Successful fork
      if (currentPID == 0) { // Child process
	isChild = 1;
	write_out_pipe = replicas[index].pipefd[1];
	break;
      } else { // Parent Process
	replicas[index].pid = currentPID;
      }
    } else {
      printf("Fork error!\n");
      return 0;
    }
  }

  // Should add asserts here for sanity?

  
  if (isChild) {
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    raise(SIGCONT);
    fprintf(write_out_pipe, "%lu\n", fib(starting_n));
  } else {
    printf("I AM THE OVERLORD\n");
    while(1) {
      currentPID = waitpid(-1, &status, 0);
    
      if (WIFEXITED(status) || WIFSIGNALED(status)) {
	finished_count++;
	if (finished_count == CHILD_NUM) {
	  break;
	}
      }

      printf("Oh, hello, %lu\n", currentPID);

      // find replica to get correct pipe
      for (index = 0; index < CHILD_NUM; index++) {
	printf("Replica pid: %lu\n", replicas[index].pid);
	if (replicas[index].pid == currentPID) {
	  printf("Scanning from: %lu\n", currentPID);
	  if (fscanf(replicas[index].pipefd[0], "%lu", &result) > 0) {
	    printf("Result: %lu\n", result);
	  }
	}
      }
    
      ptrace(PTRACE_CONT, currentPID, NULL, NULL);
    }
  }

  return 0;
}
