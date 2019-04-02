#include <stdio.h>
#include <string.h>

// Semaj's
// Both seem to work fine on my computer... 
// But Semaj's compiles for 32 bit as well.
//#define rdtscll(val) __asm__ __volatile__("rdtsc" : "=A" (val))

// Mine*
#define rdtscll(value)				       \
  __asm__ ("rdtsc\n\t"				       \
	   "shl $(32), %%rdx\n\t"		       \
	   "or %%rax, %%rdx" : "=d" (value) : : "rax")

float diff_time(unsigned long current, unsigned long last, float cpu_mhz) {
  if (current > last) {
    return (current - last) / cpu_mhz;
  } else {
    return 0.0;
  }
}

#define CPU_MHZ 3200
#define SIZE    (1 << 20)
//#define SIZE    (1 << 4)

void main() {
  unsigned long start;
  unsigned long end;
  char buffer0[SIZE] = {0};
  char buffer1[SIZE] = {0};
  int retval;

  printf("Test run. CPU_MHZ: %d\tCopy SIZE: %d\n", CPU_MHZ, SIZE);

  printf("Sup?\n");
  
  rdtscll(start);
  memcpy(buffer0, buffer1, SIZE);
  rdtscll(end);

  printf("Time to copy (usec): %f\n", diff_time(end, start, CPU_MHZ));
}
  

