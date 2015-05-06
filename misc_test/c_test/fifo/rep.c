// rep.c
// James Marshall
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv) {
  int fifo_out;
  int fifo_in;
  char * hello = "hello. What up?";
  char buffer[100];
  int fds[2];
  int retval;

  // open named pipe
  fifo_out = open("./fifo_out", O_WRONLY);
  fifo_in = open("./fifo_in", O_RDONLY);

  // request pipe fd
  write(fifo_out, hello, 7);

  retval = read(fifo_in, fds, sizeof(int) * 2);
  printf("retval : %d\n", retval);
  printf("\tFDs received: %d, %d\n", fds[0], fds[1]);

  // write / read 
  retval = read(fds[0], buffer, 7);
  if (retval < 0) {
    perror("Didn't like that, did it?");
  }

  printf("retval: %d\tmessage: %s\n", retval, buffer);

  return 0;
}
