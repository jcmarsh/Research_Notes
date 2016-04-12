#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  pid_t currentPID;
  int fd;
  char *p;

  printf("First program running\n");

  currentPID = fork();

  if (currentPID >= 0) { // Successful fork
    if (currentPID == 0) { // Child process
      if (-1 == execl("Second", "Second", NULL)) {
        perror("EXEC ERROR!");
        return -1;
      }
    }
  } else {
    printf("Fork error!\n");
    return -1;
  }

  fd = shm_open("/shared_file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd < 0) {
    perror("First open");
    exit(0);
  }
  printf("(First) fd: %d\n", fd);

  if (ftruncate(fd, 4096) == -1) {
    perror("First truncate");
    exit(0);
  }

  p = (char *) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
  printf("(First) mapped: %p\n", p);

  strcpy(p, "Hello");

  printf("(First) wrote to shared %c\n", *p);

  sleep(10);
  exit(0);
}
