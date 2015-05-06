// vote.c
// James Marshall

#include <fcntl.h> // needed to deal with pipes
#include <sys/user.h> // has pid_t
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv) {
  pid_t currentPID;
  int pipefd[2];
  int fifo_out;
  int fifo_in;
  char buffer[100];
  char * msg = "Howdy!";

  // create a pipe
  // pipe(pipefd); // Here the example works.

  // fork / exec a child
  currentPID = fork();

  if (currentPID >= 0) { // Successful fork
    if (currentPID == 0) { // Child process
      if (-1 == execl("rep", "rep", NULL)) {
        perror("EXEC ERROR!");
        return -1;
      }
    }
  } else {
    printf("Fork error!\n");
    return -1;
  }
  
  // create a named pipe
  mkfifo("./fifo_out", 0666);
  fifo_in = open("./fifo_out", O_RDONLY);
  mkfifo("./fifo_in", 0666);
  fifo_out = open("./fifo_in", O_WRONLY);

  // send fd of pipe to child through named pipe
  read(fifo_in, buffer, 100);
  printf("I have a reader!\n");

  printf("They said: %s\n", buffer);

  // create a pipe
  pipe(pipefd); // Here the example breaks.

  write(fifo_out, pipefd, sizeof(int) * 2);  

  // read / write / be happy
  write(pipefd[1], msg, 7);

  return 0;
}
