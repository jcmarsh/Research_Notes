#include <stdio.h>

// Semaj's
// Both seem to work fine on my computer... 
// But Semaj's compiles for 32 bit as well.
#define rdtscll(val) __asm__ __volatile__("rdtsc" : "=A" (val))

// Mine*
/*
#define rdtscll(value)				       \
  __asm__ ("rdtsc\n\t"				       \
	   "shl $(32), %%rdx\n\t"		       \
	   "or %%rax, %%rdx" : "=d" (value) : : "rax")
*/

void food() {
  unsigned long ticks = 80085;
  rdtscll(ticks);
  printf("Ticks: %ld\n", ticks);
}

void test_func() {
  int sum, i, count;

  for (i = 1; i <= 100; i++) {
    sum += i;
    count++;
  }

  printf("Sum: %d\tCount: %d\n", sum, count);
}

void main() {
  unsigned long start;
  unsigned long end;


  printf("Sup?\n");
  
  rdtscll(start);
  test_func();
  rdtscll(end);

  printf("Not much. You? %ld\n", end - start);
}
  

