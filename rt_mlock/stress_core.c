#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include "tas_time.h"

/*
 * Stress a single core with a low priority busy process.
 */

int main (int argc, char** argv) {
  cpu_set_t cpuset_mask;
  CPU_ZERO(&cpuset_mask);
  CPU_SET(1, &cpuset_mask);

  if (sched_setaffinity(getpid(), sizeof(cpuset_mask), &cpuset_mask ) == -1 ) {
    perror("setaffinity failed");
  }

  // Sched low
  struct sched_param param;
  param.sched_priority = 20;
  if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
    perror("Messed up setting RT scheduler");
  }

  while (1) {
    int i, j;
    printf("Stress computation\n");
    for (i = 0; i < 10000; i++) {
      for (j = 0; j < 100000; j++) {
	int t = i * j;
      }
    }
    usleep(10000);
  }
}
