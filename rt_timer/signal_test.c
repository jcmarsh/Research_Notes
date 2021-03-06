/*
 * Tests signal delivery behavior under RT_Preempt patch. A high priority task signals a
 * medium priority task while a low priority task alternates between sleep and computation.
 *
 * Surprisingly, delivery behaves better when the low priority task is running.
 */

#define _GNU_SOURCE
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tas_time.h"

#define RESTART_SIGNAL SIGRTMIN + 1

static void restartHandler(int signo, siginfo_t *si, void *unused) {
  long curr_time = generate_timestamp();
  long parent_time = (long) si->si_value.sival_ptr;
  printf("(%ld) handled in %d\n", curr_time - parent_time, getpid());
}

int main (int argc, char** argv) {
  // Make two process
  // - Child is of low priority but consumes CPU
  // - Parent is of high priority and timers the timer triggers
  
  pid_t currentPID = fork();

  if (currentPID >= 0) { // Successful fork
    if (currentPID == 0) { // Child process, low prio intensive
      cpu_set_t cpuset_mask;
      CPU_ZERO( &cpuset_mask);
      CPU_SET(1, &cpuset_mask);
      if (sched_setaffinity(getpid(), sizeof(cpuset_mask), &cpuset_mask) == -1 ) {
	perror("Failed cpu affinity");
      }

      // Sched priority low
      struct sched_param param;
      param.sched_priority = 40;
      if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
	perror("Messed up setting RT scheduler");
      }

      int i, j;
      while(1) {
	printf("child computation\n");
	for (i = 0; i < 100000; i++) {
	  for (j = 0; j < 10000; j++) {
	    int t = i * j;
	  }
	}
	printf("child sleep\n");
	sleep(5);
      }
    }
  } else {
    printf("Fork error\n");
  }
  
  currentPID = fork();

  if (currentPID >= 0) { // Successful fork
    if (currentPID == 0) { // Child process, med prio sig handler
      cpu_set_t cpuset_mask;
      CPU_ZERO( &cpuset_mask);
      CPU_SET(1, &cpuset_mask);
      if (sched_setaffinity(getpid(), sizeof(cpuset_mask), &cpuset_mask) == -1 ) {
	perror("Failed cpu affinity");
      }

      // Sched priority medium
      struct sched_param param;
      param.sched_priority = 55;
      if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
	perror("Messed up setting RT scheduler");
      }

      struct sigaction sa;
      sa.sa_flags = SA_SIGINFO;
      sigemptyset(&sa.sa_mask);
      sa.sa_sigaction = restartHandler;
      if (sigaction(RESTART_SIGNAL, &sa, NULL) == -1) {
	perror("Failed to register the restart handler");
	return -1;
      }
      
      while (1) {
	sleep(60);
      }
    }
  } else {
    printf("2nd Fork Error\n");
  }

  // parent, finally
  cpu_set_t cpuset_mask;
  CPU_ZERO( &cpuset_mask);
  CPU_SET(1, &cpuset_mask);
  if (sched_setaffinity(getpid(), sizeof(cpuset_mask), &cpuset_mask) == -1 ) {
    perror("Failed cpu affinity");
  }

  // parent times trigger events
  // Sched priority high
  struct sched_param param;
  param.sched_priority = 90;
  if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
    perror("Messed up setting RT scheduler");
  }

  usleep(10000);

  while(1) {
    long curr_time = generate_timestamp();
    union sigval time_value;
    time_value.sival_ptr = (void *)curr_time;
    int retval = sigqueue(currentPID, RESTART_SIGNAL, time_value);
    if (retval < 0) {
      perror("Something broke");
    }
    usleep(50000);
  }
}
