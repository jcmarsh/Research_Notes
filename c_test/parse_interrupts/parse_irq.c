/*
 * parse_irq.c
 *
 * James Marshall
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "taslimited.h"

#define rdtscll(value)				       \
  __asm__ ("rdtsc\n\t"				       \
	   "shl $(32), %%rdx\n\t"		       \
	   "or %%rax, %%rdx" : "=d" (value) : : "rax")

#define TARGET_ROW 15
#define TARGET_COL 2

FILE* proc_interrupts;  

long oldParse() {
  const char token[2] = " ";
  char buffer[256];
  int row_num = 0;
  int col_num = 0;
  
  char *cell_value;

  rewind(proc_interrupts);

  while(fgets(buffer, 256, proc_interrupts) > 0) {
    if (row_num == TARGET_ROW) {
      cell_value = strtok(buffer, token);
      while (cell_value != NULL) {
	cell_value = strtok(NULL, token);
	if (col_num == TARGET_COL) {
	  return atol(cell_value);
	}
	col_num++;
      }
    }
    row_num++;
  }
  return 0;
}

int main(int argc, char ** argv) {
  unsigned long curr_time;
  unsigned long last_time;
  unsigned long start_time;
  unsigned long end_time;
  int index = 0;
  int tests = 100000;
  long total_time;
  long curr_int_count;
  long last_int_count;
  long int_estimate = 0;
  

  long time_guess = 5000;
  bool results_tick[tests];
  long results_time[tests];

  cpu_speed_t cpu_speed;

  InitTAS(DEFAULT_CPU, &cpu_speed, 0);
  proc_interrupts = fopen("/proc/interrupts", "r");

  last_int_count = oldParse();
  rdtscll(start_time);
  rdtscll(last_time);
  
  for (index = 0; index < tests; index++) {
    rdtscll(curr_time);
    
    results_time[index] = curr_time - last_time;
    if (results_time[index] > time_guess) {
      results_tick[index] = true;
      //      int_estimate++;
    } else {
      results_tick[index] = false;
    }
    
    last_time = curr_time;
  }

  rdtscll(end_time);
  curr_int_count = oldParse();

  for (index = 0; index < tests; index++) {
    if (results_tick[index]) {
      printf("Timer tick: %ld\n", results_time[index]);
      int_estimate++;
    } else {
      //      printf("No Timer tick: %ld\n", results_time[index]);
    }
  }

  printf("Estimate: %ld\tActual: %ld\t in %ld\n", int_estimate, curr_int_count - last_int_count, end_time - start_time);
}
