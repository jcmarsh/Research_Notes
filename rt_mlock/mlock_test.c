#define _GNU_SOURCE

#include <sched.h>
#include <stdlib.h>
#include <time.h>
#include "force.h"
#include "tas_time.h"

/*
 * Trying to time how long mlockall takes with a busy lower priority process
 *   'sudo ./MLockTest' <- parent sleeps, 'sudo ./MLockTest_Busy' <- busy
 *
 * This program demonstrates the problem...
 *   ktimer and ksoft do not seem to have an impact.
 *   'sudo ./StressCore; sudo ./MLockTest' shows parent memory doesn't matter.
 */

int main (int argc, char** argv) {
  // Parent is of low priority but consumes CPU, spawns
  // high priority children which call mlockall

  // reserve some memory to force mlock to do work
  char * mem_bank = (char *) malloc(sizeof(char) * 1024 * 1024 * 10);

  cpu_set_t cpuset_mask;
  CPU_ZERO(&cpuset_mask);
  CPU_SET(1, &cpuset_mask);

  if (sched_setaffinity(getpid(), sizeof(cpuset_mask), &cpuset_mask ) == -1 ) {
    perror("setaffinity failed in parent");
  }

  // Sched low high
  struct sched_param param;
  param.sched_priority = 30;
  if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
    perror("Messed up setting RT scheduler");
  }

  while (1) {
    pid_t currentPID = fork();

    if (currentPID >= 0) { // Successful fork
      if (currentPID == 0) { // Child process, high prio, time mlock
	cpu_set_t cpuset_mask;
	CPU_ZERO(&cpuset_mask);
	CPU_SET(1, &cpuset_mask );

	if (sched_setaffinity(getpid(), sizeof(cpuset_mask), &cpuset_mask ) == -1 ) {
	  perror("setaffinity failed in child");
	}

	// Sched priority low
	struct sched_param param;
	param.sched_priority = 40;
	if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
	  perror("Messed up setting RT scheduler");
	}

	long last = generate_timestamp();

	/*
	 * CAUSES PROBLEMS.
	 * But MCL_CURRENT shouldn't be needed if:
	 *   MCL_FUTURE is used AND a page walk is done next.
	 */
	// mlockall(MCL_CURRENT | MCL_FUTURE);

	// Need to make sure this is equivalent as line above.
	mlockall(MCL_FUTURE);
	if (forceMaps() != 0) {
	  perror("child failed forceMaps");
	}

	long current = generate_timestamp();
	printf("Time elapsed: %ld\n", current - last);
	last = current;

	exit(0);
      } else {
	// Parent either consumes cpu or sleeps, alternates? or cla or define
#ifdef PARENT_BUSY
	int i, j;
	printf("Parent computation\n");
	for (i = 0; i < 10000; i++) {
	  for (j = 0; j < 100000; j++) {
	    int t = i * j;
	  }
	}
	usleep(10000);
#else
	printf("Parent sleep\n");
	usleep(2000000);
#endif
      }
    } else {
      printf("Fork error\n");
    }
  }
}
