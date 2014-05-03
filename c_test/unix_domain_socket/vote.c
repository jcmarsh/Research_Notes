// vote.c
// Reference: http://www.thomasstover.com/uds.html
// Reference: http://infohost.nmt.edu/~eweiss/222_book/222_book/0201433079/ch17lev1sec4.html
// James Marshall

#include <fcntl.h> // needed to deal with pipes
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/user.h>
#include <stdio.h>
#include <unistd.h>

int forkSingle() {
  pid_t currentPID;
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
  return 0;
}

int main(int argc, char ** argv) {
  struct sockaddr_un address;
  int socket_fd, connection_fd;
  socklen_t address_length;

  int pipefd[2];
  char buffer[256];
  char * msg = "Howdy!";

  if (forkSingle() < 0) {
    perror("Didn't fork.");
    return 1;
  }

  // create domain socked
  socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("Socket creation failed.");
    return 1;
  }

  // should delete previous remnants
  unlink("./fd_server");

  /* start with a clean address structure */
  memset(&address, 0, sizeof(struct sockaddr_un));

  address.sun_family = AF_UNIX;
  snprintf(address.sun_path, UNIX_PATH_MAX, "./fd_server");

  if(bind(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr_un)) != 0) {
    printf("bind() failed\n");
    return 1;
  }

  // This program will listen to connections on this socket
  if(listen(socket_fd, 5) != 0) {
    printf("listen() failed\n");
    return 1;
  }

  connection_fd = accept(socket_fd, (struct sockaddr *) &address, &address_length)) > -1) {
    printf("connection made!");

    // send fd of pipe to child through named pipe
    (fifo_in, buffer, 100);
    printf("I have a reader!\n");

  printf("They said: %s\n", buffer);

  // create a pipe
  pipe(pipefd); // Here the example breaks.

  write(fifo_out, pipefd, sizeof(int) * 2);  

  // read / write / be happy
  write(pipefd[1], msg, 7);

  return 0;
}
