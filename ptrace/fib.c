#include "utility.h"

#define CHILD_NUM 3
#define BUFF_SIZE 100

 // Represents one redundant execution, implemented as a thread
struct replica {
  pid_t pid; // The pid of the thread
  int priority; // Not yet implemented
  int pipefd[2]; // pipe to communicate with controller
  // Looks like file descriptors are going to be the way to actually deal with this.
  //  FILE* from_child; // TODO: Need better names.
  //  FILE* out_of_child;
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
  int write_out;
  int flags;
  //  FILE* write_out;
  int isChild = 0;
  int index = 0;
  unsigned long result = 0;
  char buffer[BUFF_SIZE] = {0};
  struct user_regs_struct child_regs;

  // Init three replicas
  for (index = 0; index < CHILD_NUM; index++) {
    printf("Initing index: %d\n", index);
    if (pipe(replicas[index].pipefd) == -1) {
      printf("Pipe error!\n");
      return 0;
    }

    printf("File descriptors: %d, %d\n", replicas[index].pipefd[0], replicas[index].pipefd[1]);
    flags = fcntl(replicas[index].pipefd[0], F_GETFL, 0); // Need to set to be non-blocking for reading.
    fcntl(replicas[index].pipefd[0], F_SETFL, flags | O_NONBLOCK);
    //replicas[index].from_child = fdopen(replicas[index].pipefd[0], "r");
    //replicas[index].out_of_child = fdopen(replicas[index].pipefd[1], "w");
    //if (replicas[index].from_child == NULL || replicas[index].out_of_child == NULL) {
    //  printf("FD to FILE* for pipe error!\n");
    //  printf("\t%s\n", strerror(errno));
    //  return 0;
    //}

    replicas[index].pid = -1;
    replicas[index].priority = -1;
  }
    
    
  // Fork three children
  for (index = 0; index < CHILD_NUM; index++) {
    currentPID = fork();
    
    if (currentPID >= 0) { // Successful fork
      if (currentPID == 0) { // Child process
	isChild = 1;
	write_out = replicas[index].pipefd[1];
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
    snprintf(buffer, BUFF_SIZE, "%lu", fib(starting_n));
    write(write_out, buffer, BUFF_SIZE);
    //    fprintf(write_out, "%lu\n", fib(starting_n));
  } else {
    printf("I AM THE OVERLORD\n");
    while(1) {
      // Check for output
      // Right now everything is done on the ptrace signals... not sure if that is good or bad.

      // ptrace handling
      currentPID = waitpid(-1, &status, 0); // Consider adding WNOHANG as option

      printf("Oh, hello, %lu\n", currentPID);

      if (ptrace(PTRACE_GETREGS, currentPID, NULL, &child_regs) < 0) {
	printf("GETREGS error.\n");
	//	return 0;
      }

      printRegs(&child_regs);

      // find replica to get correct pipe
      for (index = 0; index < CHILD_NUM; index++) {
	if (replicas[index].pid == currentPID) {
	  printf("Scanning from: %lu\n", currentPID);
	  read(replicas[index].pipefd[0], buffer, BUFF_SIZE);
	  //if (fscanf(replicas[index].from_child, "%lu", &result) > 0) {
	  //  printf("Result: %lu\n", result);
	  //}
	}
      }
    
      // Print out the buffer?
      for (index = 0; index < BUFF_SIZE; index++) {
	if (buffer[index] != '\0') {
	  printf("%c", buffer[index]);
	} else {
	  printf("\n");
	  break;
	}
      }

      if (WIFEXITED(status) || WIFSIGNALED(status)) {
	printf("Exit status from pid: %lu\n", currentPID);
	finished_count++;
	if (finished_count == CHILD_NUM) {
	  break;
	}
      }

      ptrace(PTRACE_SYSCALL, currentPID, NULL, NULL);
    }
  }

  return 0;
}
