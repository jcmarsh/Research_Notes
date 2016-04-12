/*
 * Does stuff with shared memory
 *
 * James Marshall
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  int fd;
  char *p;

  printf("Second program starting.\n");

  sleep(5);

  fd = shm_open("/shared_file", O_RDWR, 0);
  if (fd < 0) {
    perror("Second open");
    exit(0);
  }
  printf("(Second) fd: %d\n", fd);

  p = (char *) mmap(NULL, 4096, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
  printf("(Second) mapped: %p\n", p);

  printf("What's in the share?: %c\n", *p);

  close(fd);
  exit(0);
}
