#include "utility.h"

#define CHILD_NUM 3
#define BUFF_SIZE 100
#define SEC 0
#define USEC 500000

 // Represents one redundant execution, implemented as a thread
struct replica {
  pid_t pid; // The pid of the thread
  int priority; // Not yet implemented
  int pipefd[2]; // pipe to communicate with controller
  // Possibly put a pointer to entry function
  unsigned long last_result;
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
  struct replica replicas[CHILD_NUM];
  int write_out;
  int flags;
  int isChild = 0;
  int index = 0;
  int status = -1;
  char buffer[BUFF_SIZE] = {0};
  int finished_count = 0;
  //  struct user_regs_struct child_regs;

  // select stuff
  int nfds = 0;
  fd_set read_fds;
  struct timeval tv;
  int retval;

  FD_ZERO(&read_fds);
  tv.tv_sec = SEC;
  tv.tv_usec = USEC;

  // Init three replicas
  for (index = 0; index < CHILD_NUM; index++) {
    printf("Initing index: %d\n", index);
    if (pipe(replicas[index].pipefd) == -1) {
      printf("Pipe error!\n");
      return 0;
    }

    printf("File descriptors: %d, %d\n", replicas[index].pipefd[0], replicas[index].pipefd[1]);
    
    // Need to set to be non-blocking for reading.
    flags = fcntl(replicas[index].pipefd[0], F_GETFL, 0);
    fcntl(replicas[index].pipefd[0], F_SETFL, flags | O_NONBLOCK);

    // nfds should be the highes file descriptor, plus 1
    // TODO: This may have to be changed for when signal fd is added
    if (replicas[index].pipefd[0] >= nfds) {
      nfds = replicas[index].pipefd[0] + 1;
    }
    // Set to select on pipe's file descriptor
    FD_SET(replicas[index].pipefd[0], &read_fds);

    replicas[index].pid = -1;
    replicas[index].priority = -1;
    replicas[index].last_result = 0;
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
    //    raise(SIGCONT);
    snprintf(buffer, BUFF_SIZE, "%lu", fib(starting_n));
    write(write_out, buffer, BUFF_SIZE);
    //    fprintf(write_out, "%lu\n", fib(starting_n));
  } else {
    printf("I AM THE OVERLORD\n");
    while(1) {
      if (finished_count == CHILD_NUM) {
	// All are finished executing, time to vote and join.
	break;
      }

      // Insert an error?
      // Always insert an error for the first one
      kill(replicas[2].pid, SIGUSR1);

      // Check for stopped processes
      // PROBLEMS HERE.
      currentPID = waitpid(-1, &status, WNOHANG);
      printf("PID returned: %u\n", currentPID);
      if (WIFSTOPPED(status)) {
	printf("STOPPED\n");
      }
      if (WSTOPSIG(status) == SIGUSR1) {
	printf("Hells yeah!\n");
      }
      //cont.

      retval = select(nfds, &read_fds, NULL, NULL, &tv);

      tv.tv_sec = SEC;
      tv.tv_usec = USEC;


      if (retval == -1) {
	perror("select()");
	// I think this behaviour is off because the fds do not block.
	//      } else if (retval == 0) {
	//	printf("Nothing...\n");
      } else {
	printf("Something to read: %d\n", retval);
	// Data to read! Loop through and print
	for (index = 0; index < CHILD_NUM; index++) {
	  retval = read(replicas[index].pipefd[0], buffer, BUFF_SIZE);
	  if (retval > 0) {
	    replicas[index].last_result = atol(buffer);
	    printf("Result %d:%lu\n", index, replicas[index].last_result);
	    memset(buffer, 0, BUFF_SIZE);
	    finished_count++;
	  }
	}
      }
    }

    printf("All Done!\n");

    for (index = 0; index < CHILD_NUM; index++) {
      printf("\tResult %d: %lu\n", index, replicas[index].last_result);
    }
  }

  return 0;
}
