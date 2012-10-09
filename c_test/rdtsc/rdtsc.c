#include <stdio.h>

#define rdtscll(value)				       \
  __asm__ ("rdtsc\n\t"				       \
	   "shl $(32), %%rdx\n\t"		       \
	   "or %%rax, %%rdx" : "=d" (value) : : "rax")

void food() {
  unsigned long ticks = 80085;
  rdtscll(ticks);
  printf("Ticks: %ld\n", ticks);
}

void main() {
  printf("Sup?\n");
  food();
  printf("Not much. You?\n");
}
  

