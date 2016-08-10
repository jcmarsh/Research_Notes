/**
 *  Testing the order of data arrival on pipes across cores.
 *
 *  James Marshall
 */

#include <fcntl.h> // needed to deal with pipes
#include <sys/user.h> // has pid_t
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

#include "taslimited.h"

#define PIPE_COUNT 4

int main(int argc, char ** argv) {
  pid_t currentPID;
  int index;

  int pipes[PIPE_COUNT][2];

  for (index = 0; index < PIPE_COUNT; index++) {
    if (0 != pipe(pipes[index])) {
      perror("Pipe creation problem");
      return -1;
    }
  }

  currentPID = fork();

  if (currentPID >= 0) { // Successful fork
    if (currentPID == 0) { // Child process
      struct timeval select_timeout;
      fd_set select_set;
      char* buffer[1024];
      int retval, last_read = PIPE_COUNT - 1;

      // init, set core
      InitTAS(1, 30);

      while (1) {
	// select over pipes
	select_timeout.tv_sec = 1;
	select_timeout.tv_usec = 0;

	FD_ZERO(&select_set);
	for (index = 0; index < PIPE_COUNT; index++) {
	  FD_SET(pipes[index][0], &select_set);
	}

	retval = select(FD_SETSIZE, &select_set, NULL, NULL, &select_timeout);

	// try to read in order 0-3, alert if that didn't happen
	if (retval > 0) {
	  for (index = 0; index < PIPE_COUNT; index++) {
	    if (FD_ISSET(pipes[index][0], &select_set)) {
	      if (last_read != ((index - 1 + PIPE_COUNT) % PIPE_COUNT)) {
		printf("Out of order pipe detected: %d, previous %d, expected %d\n", index, last_read, ((index - 1 + PIPE_COUNT) % PIPE_COUNT));
	      }
	      last_read = index;
	      read(pipes[index][0], buffer, 1024);
	      printf("\tChild read on %d\n", index);
	    }
	  }
	}	      
      }
    } else { // Parent process
      // init, set core
      InitTAS(0, 35);

      while (1) {
	printf("Parent writing.\n");
	// write data to pipes in order 0-3
	for (index = 0; index < PIPE_COUNT; index++) {
	  write(pipes[index][1], "Hello", 6);
	}

	// take a break
	sleep(1);
      }
    }
  } else {
    printf("Fork error!\n");
    return -1;
  }
  
  return 0;
}
