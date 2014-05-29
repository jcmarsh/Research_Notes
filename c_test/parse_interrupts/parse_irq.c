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

/*
 * 14 - Non-maskable interrupts
 * 15 - Local timer interrupts
 * 17 - Performance monitoring interrupts
 * 19 - Rescheduling interrupts
 * 20 - Function call interrupts
 * 21 - TLB shootdowns
 * 25 - Machine check polls
 */

#define MAX_RESULT_COUNT 100000
#define TARGET_COL 2 // Should be cpu 2

FILE* proc_interrupts;  

long oldParse() {
  const char token[2] = " ";
  char buffer[256];
  int row_num = 0;
  int col_num = 0;
  
  char *cell_value;
  //int target_num = 6;
  //int target_rows[] = {14, 15, 17, 19, 20, 21, 25};
  int target_num = 1;
  int target_rows[] = {25};
  int row_index = 0;
  long total = 0;

  rewind(proc_interrupts);

  while(fgets(buffer, 256, proc_interrupts) > 0) {
    if (row_num == target_rows[row_index]) {
      col_num = 0;
      row_index++;
      cell_value = strtok(buffer, token);
      while (cell_value != NULL) {
	cell_value = strtok(NULL, token);
	if (col_num == TARGET_COL) {
	  total += atol(cell_value);
	  if (row_index >= target_num) {
	    return total;
	  }
	}
	col_num++;
      }
    }
    row_num++;
  }
  return total;
}

int main(int argc, char ** argv) {
  unsigned long curr_time;
  unsigned long last_time;
  unsigned long start_time;
  unsigned long end_time;
  long index = 0;
  int next_result_index = 0;
  long curr_int_count;
  long last_int_count;

  int result_count = 0;
  int threshold = 0; // anything over this will be recorded (set by args)

  long results_time[MAX_RESULT_COUNT];

  cpu_speed_t cpu_speed;

  // Parse args
  if (argc == 3) {
    result_count = atoi(argv[1]);
    threshold = atoi(argv[2]);
    if (result_count > MAX_RESULT_COUNT) {
      printf("To many results requested.\n");
      return 0;
    }
  } else {
    printf("Usage: ParseIrq <result_count> <threshold>\n");
    return 0;
  }

  InitTAS(DEFAULT_CPU, &cpu_speed, 0);
  proc_interrupts = fopen("/proc/interrupts", "r");

  last_int_count = oldParse();
  rdtscll(start_time);
  rdtscll(last_time);
  
  while (next_result_index < result_count) {
    rdtscll(curr_time);
    
    results_time[next_result_index] = curr_time - last_time;
    if (results_time[next_result_index] > threshold) {
      next_result_index++;
    }
    
    last_time = curr_time;
  }

  rdtscll(end_time);
  curr_int_count = oldParse();

  printf("Estimate: %d\tActual: %ld\t in %ld\n", next_result_index, curr_int_count - last_int_count, end_time - start_time);
  for (index = 0; index < next_result_index; index++) {
    printf("%06ld\n", results_time[index]);
  }
  printf("Estimate (%d - threshold): %d\tActual: %ld\t in %ld\n", threshold, next_result_index, curr_int_count - last_int_count, end_time - start_time);
}
