// vote.c
// Reference: http://www.thomasstover.com/uds.html
// Reference: http://infohost.nmt.edu/~eweiss/222_book/222_book/0201433079/ch17lev1sec4.html
// Reference: http://stackoverflow.com/questions/4489433/sending-file-descriptor-over-unix-domain-socket-and-select
// James Marshall

#include <fcntl.h> // needed to deal with pipes
#include <linux/un.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include <sys/un.h>
#include <sys/user.h>
#include <stdio.h>
#include <string.h>
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

// From the SO post
int sendfd(int sock, int fd_out, int fd_in)
{
  struct msghdr hdr;
  struct iovec data;

  char cmsgbuf[CMSG_SPACE(sizeof(int) * 2)];

  char dummy = '*';
  data.iov_base = &dummy;
  data.iov_len = sizeof(dummy);

  memset(&hdr, 0, sizeof(hdr));
  hdr.msg_name = NULL;
  hdr.msg_namelen = 0;
  hdr.msg_iov = &data;
  hdr.msg_iovlen = 1;
  hdr.msg_flags = 0;

  hdr.msg_control = cmsgbuf;
  hdr.msg_controllen = CMSG_LEN(sizeof(int) * 2);

  struct cmsghdr* cmsg = CMSG_FIRSTHDR(&hdr);
  cmsg->cmsg_len   = CMSG_LEN(sizeof(int) * 2);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type  = SCM_RIGHTS;

  ((int*)CMSG_DATA(cmsg))[0] = fd_out;
  ((int*)CMSG_DATA(cmsg))[1] = fd_in;

  int n = sendmsg(sock, &hdr, 0);

  if(n == -1) {
    perror("sendmsg() failed");
  }

  return n;
}

int main(int argc, char ** argv) {
  struct sockaddr_un address;
  int socket_fd, connection_fd;
  socklen_t address_length;

  int pipefd[2];
  int pipe_in[2];
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

  if ((connection_fd = accept(socket_fd, (struct sockaddr *) &address, &address_length)) > -1) {
    printf("connection made!\n");

    // create a pipe
    pipe(pipefd);  
    pipe(pipe_in);

    sendfd(connection_fd, pipefd[0], pipe_in[1]); // send read end to client
  }

  printf("Writing to pipe!\n");

  // read / write / be happy
  write(pipefd[1], msg, 7);

  read(pipe_in[0], buffer, 256);

  printf("Voter recieved back: %s\n", buffer);

  close(socket_fd);
  unlink("./fd_server");

  return 0;
}
