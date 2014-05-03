// rep.c
// James Marshall
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

int recvfd(int sock, int* received_fd) {
  struct msghdr hdr;
  struct iovec data;
  int retval;

  char cmsgbuf[CMSG_SPACE(sizeof(int))];

  char dummy = '*';

  data.iov_base = &dummy;
  data.iov_len = sizeof(dummy);

  memset(&hdr, 0, sizeof(hdr));
  hdr.msg_name = NULL;
  hdr.msg_namelen = 0;
  hdr.msg_iov = &data;
  hdr.msg_iovlen = 1;
  hdr.msg_flags = 0;

  struct cmsghdr *cmsg = malloc(CMSG_LEN(sizeof(int)));
  hdr.msg_control = cmsg;
  hdr.msg_controllen = CMSG_LEN(sizeof(int));

  retval = recvmsg(sock, &hdr, 0);
  if (retval < 0) {
    perror("RECVMSG error");
    return(-1);
  }

  *received_fd = *(int *)CMSG_DATA(cmsg);
}

int main(int argc, char ** argv) {
  int fds_in;
  int retval;
  struct sockaddr_un address;
  int  socket_fd, nbytes;
  char buffer[256];
  struct msghdr hdr;
  struct iovec data;

  socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
  if(socket_fd < 0) {
    perror("socket() failed\n");
    return 1;
  }

  /* start with a clean address structure */
  memset(&address, 0, sizeof(struct sockaddr_un));
 
  address.sun_family = AF_UNIX;
  snprintf(address.sun_path, UNIX_PATH_MAX, "./fd_server");

  if(connect(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr_un)) != 0) {
    printf("connect() failed\n");
    return 1;
  }

  printf("\tRetrieve FD\n");
  retval = recvfd(socket_fd, &fds_in);
  printf("\tNew fd: %d\n", fds_in);

  retval = read(fds_in, buffer, 256);
  if (retval < 0) {
    perror("Read failure");
  }

  printf("MESSAGE FROM SERVER: %d, %s\n", retval,  buffer);

  close(socket_fd);

  return 0;
}
