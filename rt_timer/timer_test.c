#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tas_time.h"

/*
 * Demonstrates delayed timer activations for a RT process by a
 * lower priority RT process on the same core.
 */

int main (int argc, char** argv) {
  // Make two process
  // - Child is of low priority but consumes CPU
  // - Parent is of high priority and timers the timer triggers

  pid_t currentPID = fork();

  if (currentPID >= 0) { // Successful fork
    if (currentPID == 0) { // Child process, low prio intensive
      cpu_set_t cpuset_mask;
      CPU_ZERO(&cpuset_mask);
      CPU_SET(1, &cpuset_mask );

      if (sched_setaffinity(getpid(), sizeof(cpuset_mask), &cpuset_mask ) == -1 ) {
	perror("setaffinity failed in child");
      }

      // Sched priority low
      struct sched_param param;
      param.sched_priority = 30;
      if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
      	perror("Messed up setting RT scheduler");
      }

      int i, j;
      while(1) {
	printf("child computation\n");
	for (i = 0; i < 10000; i++) {
	  for (j = 0; j < 10000; j++) {
	    int t = i * j;
	  }
	}
	printf("child sleep\n");
	usleep(200000);
      }
    }
  } else {
    printf("Fork error\n");
  }

  cpu_set_t cpuset_mask;
  CPU_ZERO(&cpuset_mask);
  CPU_SET(1, &cpuset_mask);

  if (sched_setaffinity(getpid(), sizeof(cpuset_mask), &cpuset_mask ) == -1 ) {
    perror("setaffinity failed in parent");
  }

  // parent times trigger events
  // Sched priority high
  struct sched_param param;
  param.sched_priority = 40;
  if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
    perror("Messed up setting RT scheduler");
  }

  // create timer
  int   timeout_fd = timerfd_create(CLOCK_MONOTONIC, 0);
  if (timeout_fd == -1) {
    perror("timer_create failed");
    return -1;
  }

  // Set timer
  struct itimerspec its;
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 0;
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 12000000;
  if (timerfd_settime(timeout_fd, 0, &its, NULL) == -1) {
    perror("timerfd_settime failed");
  }
  long last = generate_timestamp();

  struct timeval select_timeout;
  fd_set select_set;
  while(1) {
    select_timeout.tv_sec = 5;
    select_timeout.tv_usec = 0;

    FD_ZERO(&select_set);
    FD_SET(timeout_fd, &select_set);

    int read_ret;
    int retval = select(FD_SETSIZE, &select_set, NULL, NULL, &select_timeout);
    if (retval > 0) {
      if (FD_ISSET(timeout_fd, &select_set)) {
	unsigned long result = 0;
	retval = read(timeout_fd, &result, sizeof(unsigned long));
	if (retval > 0) {
	  // read, print time elapsed, and reset
	  long current = generate_timestamp();
	  printf("Time elapsed: %ld\n", current - last);
	  last = current;
	  its.it_interval.tv_sec = 0;
	  its.it_interval.tv_nsec = 0;
	  its.it_value.tv_sec = 0;
	  its.it_value.tv_nsec = 12000000;
	  if (timerfd_settime(timeout_fd, 0, &its, NULL) == -1) {
	    perror("timerfd_settime failed");
	  }
	}
      }
    }
  }
}
