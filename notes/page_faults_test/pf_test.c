// James Marshall
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> // Needed for mlockall()
#include <unistd.h> // needed for sysconf(int name);
#include <malloc.h>
#include <sys/time.h>
#include <sys/resource.h>

// https://rt.wiki.kernel.org/index.php/Dynamic_memory_allocation_example

void alloc_test(void) {
  char *buffer;
  int i;

  buffer = malloc(sizeof(char) * 1024 * 100);

  for (i = 0; i < 1024 * 100; i++) {
    buffer[i] = 'b';
  }

  free(buffer);
}

void main(void) {
  struct rusage usage_stats;
  char *reserve;
  char *buffer;
  int ret_val, i;
  int kilos = 100;

  ret_val = mlockall(MCL_CURRENT | MCL_FUTURE);
  if (ret_val < 0) {
    perror("mlockall");
  }

  getrusage(RUSAGE_SELF, &usage_stats);
  printf("Page Fault check 0: %ld - %ld\n", usage_stats.ru_majflt,
	 usage_stats.ru_minflt);

  // Turn off malloc trimming.
  mallopt (M_TRIM_THRESHOLD, -1);
  // Turn off mmap usage.
  mallopt (M_MMAP_MAX, 0);

  reserve = malloc(sizeof(char) * 1024 * 400);
  free(reserve);

  getrusage(RUSAGE_SELF, &usage_stats);
  printf("Page Fault check 1: %ld - %ld\n", usage_stats.ru_majflt,
	 usage_stats.ru_minflt);


  buffer = malloc(sizeof(char) * 1024 * kilos);

  getrusage(RUSAGE_SELF, &usage_stats);
  printf("Page Fault check 2: %ld - %ld\n", usage_stats.ru_majflt,
	 usage_stats.ru_minflt);

  for (i = 0; i < kilos * 1024; i++) {
    buffer[i] = 'a';
  }

  getrusage(RUSAGE_SELF, &usage_stats);
  printf("Page Fault check 3: %ld - %ld\n", usage_stats.ru_majflt,
	 usage_stats.ru_minflt);

  for (i = 0; i < 50; i++) {
    alloc_test();
  }

  getrusage(RUSAGE_SELF, &usage_stats);
  printf("Page Fault check 3: %ld - %ld\n", usage_stats.ru_majflt,
	 usage_stats.ru_minflt);
}
