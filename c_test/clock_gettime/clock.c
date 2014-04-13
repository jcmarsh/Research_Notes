#include <stdio.h>
#include <time.h>

void test_func() {
  int sum, i, count;

  for (i = 1; i <= 100; i++) {
    sum += i;
    count++;
  }

  // It is likely that the printf takes the most time.
  printf("Sum: %d\tCount: %d\n", sum, count);
}

void main() {
  struct timespec start;
  struct timespec end;
  long elapsed_seconds = 0;
  long elapsed_nseconds = 0;

  printf("Sup?\n");
  
  clock_gettime(CLOCK_REALTIME, &start);
  test_func();
  clock_gettime(CLOCK_REALTIME, &end);

  elapsed_seconds = end.tv_sec - start.tv_sec;
  elapsed_nseconds = end.tv_nsec - start.tv_nsec;

  printf("Time for function all: %lds and %ldns\n", elapsed_seconds, elapsed_nseconds);
}
  

