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
  int fifooo;

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
  mkfifo("./fifo", 0666);
  fifooo = open("./fifo", O_WRONLY);

  // create a pipe
  pipe(pipefd);

  // send fd of pipe to child through named pipe
  read(fifooo);
  printf("I have a reader!\n");

  // read / write / be happy

  return 0;
}
