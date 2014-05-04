/*
 * test.c
 *
 * Just a test program to demonstrate log_serv.
 *
 * James Marshall
 */
#include <fcntl.h>
#include <linux/un.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include <sys/un.h>
#include <sys/user.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char ** argv) {
  int fds_in;
  int retval;
  struct sockaddr_un address;
  int  socket_fd, nbytes;
  char buffer[256];
  struct msghdr hdr;
  struct iovec data;

  socket_fd = socket(PF_UNIX, SOCK_DGRAM, 0);
  if(socket_fd < 0) {
    perror("socket() failed\n");
    return 1;
  }

  /* start with a clean address structure */
  memset(&address, 0, sizeof(struct sockaddr_un));
 
  address.sun_family = AF_UNIX;
  snprintf(address.sun_path, UNIX_PATH_MAX, SERVER_NAME); // should be define in the header... which doesn't exist yet

  // sendto

  close(socket_fd);

  return 0;
}
