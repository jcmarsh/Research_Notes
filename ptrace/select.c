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

// Modify the register structure to have one (uniformily distributed) bit flip.
void injectRegError(struct user_regs_struct * regs) {


  // Create a new bitmask with 1 bit set true... xor 
  // Ought to be a long, no?
  unsigned long error_mask = 1; // Shift right random number of times (between 0 and WORDSIZE)
  int byte_num = 0;
  int reg_num = 0;
  int reg_pick = 0;
  int bit_pick = 0;

  byte_num =  __WORDSIZE / 8;
  reg_num =  sizeof(struct user_regs_struct) / byte_num;

  //  printf("byte_num: %d\treg_num: %d\n", byte_num, reg_num);

  // This does not give a unifrom distribution... close enough?
  // Check out http://en.wikipedia.org/wiki/Mersenne_twister
  reg_pick = rand() % reg_num;
  bit_pick = rand() % __WORDSIZE;
  
  printf("reg_pick: %d\tbit_pick: %d\n", reg_pick, bit_pick);

  //  printf("hmmm: %lu\n", *((unsigned long *)regs + reg_pick));
  printf("Old value: %lX\tNew value: %lX\n", *((unsigned long *)regs + reg_pick), *((unsigned long *)regs + reg_pick) ^ (error_mask << bit_pick));
  *((unsigned long *)regs + reg_pick) = *((unsigned long *)regs + reg_pick) ^ (error_mask << bit_pick);
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
  pid_t currentPID = 0;
  struct replica replicas[CHILD_NUM];
  int write_out;
  int flags;
  int isChild = 0;
  int index = 0;
  int status = -1;
  char buffer[BUFF_SIZE] = {0};
  int finished_count = 0;

  // select stuff
  int nfds = 0;
  fd_set read_fds;
  struct timeval tv;
  int retval;

  // Error!
  int signal = -1;
  int insert_error = 1;
  struct user_regs_struct copy_regs;

  // Final reporting
  char outcome = 'B'; // B for BENIGN
  unsigned long prev_result;

  int countdown = 20;

  FD_ZERO(&read_fds);
  tv.tv_sec = SEC;
  tv.tv_usec = USEC;

  //
  srand(time(NULL));

  // Init three replicas
  for (index = 0; index < CHILD_NUM; index++) {
    //    printf("Initing index: %d\n", index);
    if (pipe(replicas[index].pipefd) == -1) {
      printf("Pipe error!\n");
      return 0;
    }

    //    printf("File descriptors: %d, %d\n", replicas[index].pipefd[0], replicas[index].pipefd[1]);
    
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
    // printf("I AM THE OVERLORD\n");
    while(1) {
      if (finished_count == CHILD_NUM) {
	// All are finished executing, time to vote and join.
	break;
      }

      countdown--;
      if (countdown < 0) {
	// Time's up
	outcome = 'T'; // T for TIMEOUT
	break;
      }

      // Insert an error?
      // Always insert an error for the first one
      if (insert_error) {
	kill(replicas[2].pid, SIGCONT);
	//insert_error = 0;
      }

      // Check for stopped processes
      currentPID = waitpid(-1, &status, WNOHANG);
      //      printf("PID returned: %d\n", currentPID);
      if (WIFEXITED(status)) {
	//	printf("EXITED %d\n", currentPID);
      }
      if (WIFSIGNALED(status)) {
	//	printf("SIGNALED %d\n", currentPID);
      }
      if (WIFSTOPPED(status)) {
	signal = WSTOPSIG(status);
	//	printf("STOPPED %d\n", currentPID);
	//	printf("\tSignal: %d\n", WSTOPSIG(status));
	switch (signal) {
	case SIGCONT:
	  // Hopefully was the signal we sent... so insert an error
	  // NEED TO CHECK THIS. I think it is the source of the Input/Output error on setregs
	  // I don't think I can count on this. Should send an alarm? or maybe a user defined signal.
	  if (insert_error == 0) {
	    // Do nothing, error already has been inserted
	  } else {
	    insert_error = 0;
	    
	    if (ptrace(PTRACE_GETREGS, currentPID, NULL, &copy_regs) < 0) {
	      perror("GETREGS error.");
	    }

	    // Inject an error: for now a bit flip in a register
	    injectRegError(&copy_regs);

	    if(ptrace(PTRACE_SETREGS, currentPID, NULL, &copy_regs) < 0) {
	      perror("SETREGS error:");
	    }
	  }
	  ptrace(PTRACE_CONT, currentPID, NULL, NULL);
	  break;
	case SIGILL:
	  // Illegal Instruction: Kill process. #4
	  kill(currentPID, SIGKILL);
	  outcome = 'C'; // C for CRASH
	  finished_count++;
	  break;
	case SIGBUS:
	  // Bus error (bad memory access): Kill process. #7
	  kill(currentPID, SIGKILL);
	  outcome = 'C'; // C for CRASH
	  finished_count++;
	  break;
	case SIGSEGV:
	  // Invalid memory reference: Kill process. #11
	  kill(currentPID, SIGKILL);
	  outcome = 'C'; // C for CRASH
	  finished_count++;
	  break;

	default:
	  printf("Unhandled signal: %d\n", signal);
	  break;
	}
      }

      status = -1;
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
	//	printf("Something to read: %d\n", retval);
	// Data to read! Loop through and print
	for (index = 0; index < CHILD_NUM; index++) {
	  retval = read(replicas[index].pipefd[0], buffer, BUFF_SIZE);
	  if (retval > 0) {
	    replicas[index].last_result = atol(buffer);
	    //	    printf("Result %d:%lu\n", index, replicas[index].last_result);
	    memset(buffer, 0, BUFF_SIZE);
	    finished_count++;
	  }
	}
      }
    }

    //    printf("All Done!\n");

    prev_result = replicas[0].last_result;
    for (index = 0; index < CHILD_NUM; index++) {
      printf("\tResult %d: %lu\n", index, replicas[index].last_result);
      if (prev_result != replicas[index].last_result) {
	if (outcome != 'C' && outcome != 'T') {
	  outcome = 'S'; // S for SILENT DATA CORRUPTION!
	}
      }
      prev_result = replicas[index].last_result;
    }
    printf("RESULT: %c\n", outcome);
  }

  return 0;
}
