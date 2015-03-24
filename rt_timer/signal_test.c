#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RESTART_SIGNAL SIGRTMIN + 1

#define rdtscll(value)				       \
  __asm__ ("rdtsc\n\t"				       \
	   "shl $(32), %%rdx\n\t"		       \
	   "or %%rax, %%rdx" : "=d" (value) : : "rax")

long generate_timestamp( void ) {
  long ts;
  rdtscll( ts );
  return ts;
}

static void restartHandler(int signo, siginfo_t *si, void *unused) {
	long curr_time = generate_timestamp();
	long parent_time = (long) si->si_value.sival_ptr;
	printf("(%ld)\n", curr_time - parent_time);
}

int main (int argc, char** argv) {
	// Make two process
	// - Child is of low priority but consumes CPU
	// - Parent is of high priority and timers the timer triggers

	pid_t currentPID = fork();

	if (currentPID >= 0) { // Successful fork
		if (currentPID == 0) { // Child process, low prio intensive
			// Sched priority low
			struct sched_param param;
			param.sched_priority = 40;
			if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
				perror("Messed up setting RT scheduler");
			}

			// Setup signal handler
			struct sigaction sa;
			sa.sa_flags = SA_SIGINFO;
			sigemptyset(&sa.sa_mask);
			sa.sa_sigaction = restartHandler;
			if (sigaction(RESTART_SIGNAL, &sa, NULL) == -1) {
				perror("Failed to register the restart handler");
				return -1;
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
				usleep(2000000);
			}
		}
	} else {
		printf("Fork error\n");
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