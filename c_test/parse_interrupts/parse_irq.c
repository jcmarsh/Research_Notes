/*
 * parse_irq.c
 *
 * James Marshall
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define rdtscll(value)				       \
  __asm__ ("rdtsc\n\t"				       \
	   "shl $(32), %%rdx\n\t"		       \
	   "or %%rax, %%rdx" : "=d" (value) : : "rax")

#define TARGET_ROW 15
#define TARGET_COL 2

int oldParse() {
  const char token[2] = " ";
  FILE* proc_interrupts;  
  char buffer[256];
  int row_num = 0;
  int col_num = 0;
  
  char *cell_value;

  proc_interrupts = fopen("/proc/interrupts", "r");

  while(fgets(buffer, 256, proc_interrupts) > 0) {
    if (row_num == TARGET_ROW) {
      printf("Line %d: %s\n", row_num, buffer);
      cell_value = strtok(buffer, token);
      printf("Sigh: %s\n", cell_value);
      while (cell_value != NULL) {
	cell_value = strtok(NULL, token);
	if (col_num == TARGET_COL) {
	  printf("cell_value: %s\n", cell_value);
	}
	col_num++;
      }
    }
    row_num++;
  }
}

int main(int argc, char ** argv) {
  unsigned long start;
  unsigned long end;
  int index = 0;
  int tests = 100;
  long total_time;

  for (index = 0; index < tests; index++) {
    rdtscll(start);
    oldParse();
    rdtscll(end);
    total_time += end - start;
  }
  
  printf("Average: %lf\n", (1.0 * total_time) / tests);
}
