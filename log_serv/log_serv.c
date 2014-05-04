/*
 * log_serv.c
 *
 * A simple server that print out whatever it receiveds over a domain socket.
 * Intended to allow for lower latency prints (than printf)
 *
 * Reference: http://www.thomasstover.com/uds.html
 *
 * James Marshall May 4, 2014
 */

#include <fcntl.h>
#include <linux/un.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/user.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_NAME "~/research/Log_Server"

int main(int argc, char ** argv) {
  struct sockaddr_un address;
  int socket_fd, connection_fd;
  socklen_t address_length;

  // create domain socked
  socket_fd = socket(PF_UNIX, SOCK_DGRAM, 0); // Connectionless
  if (socket_fd < 0) {
    perror("Socket creation failed.");
    return 1;
  }

  // should delete previous remnants
  unlink(SERVER_NAME);

  /* start with a clean address structure */
  memset(&address, 0, sizeof(struct sockaddr_un));

  address.sun_family = AF_UNIX;
  snprintf(address.sun_path, UNIX_PATH_MAX, SERVER_NAME);

  if(bind(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr_un)) != 0) {
    perror("bind() failed");
    return 1;
  }

  // This program will listen to connections on this socket
  if(listen(socket_fd, 5) != 0) {
    perror("listen() failed");
    return 1;
  }

  //  ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
  //		   struct sockaddr *src_addr, socklen_t *addrlen);

  while(1) {
    //    recvfrom(socket_fd, buffer, 
  }

  close(socket_fd);
  unlink(SERVER_NAME);

  return 0;
}
