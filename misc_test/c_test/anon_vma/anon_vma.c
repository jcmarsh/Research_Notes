
// From Daniel Frost, same as patch https://lkml.org/lkml/2012/8/15/765)
// Run this program, and check for growth in /proc/slabinfo anon_vma

// This program isn't causing the expected behavior. Maybe I'm not waiting long enough?

#include <unistd.h>

int main(int argc, char *argv[])
{
  pid_t pid;
  while (1) {
    pid = fork();
    if (pid == -1) {
      /* error */
      return 1;
    }
    if (pid) {
      /* parent */
      sleep(2);
      break;
    }
    else {
      /* child */
      sleep(1);
    }
  }
  return 0;
}
