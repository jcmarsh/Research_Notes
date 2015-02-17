#define _GNU_SOURCE

#include <sched.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

#define rdtscll(value)				       \
  __asm__ ("rdtsc\n\t"				       \
	   "shl $(32), %%rdx\n\t"		       \
	   "or %%rax, %%rdx" : "=d" (value) : : "rax")

int lockItUp(void) {
	// Must be super user.
	if (mlockall(MCL_CURRENT || MCL_FUTURE) == -1) { // Lock current pages and future ones.
		perror("mlockall failed");
		return -1;
	}

	return 0;
}

int cpu_bind( const pid_t pid, const int cpu ) {
 	cpu_set_t cpuset_mask;
 	// zero out the cpu set
 	CPU_ZERO( &cpuset_mask );
 	// bind the processor with the zeroed mask 
 	CPU_SET( cpu, &cpuset_mask );

	// set the affinity of the process to the mask containing the processor
	// this effectively binds the process to the processor
	if( sched_setaffinity( pid, sizeof(cpuset_mask), &cpuset_mask ) == -1 ) {
		return -1;
	}

	return 0;
}

void initProcess(void) {
	if (cpu_bind(getpid(), 1) < 0) {
		perror("Parent cpu bind failed");
	}
	if (lockItUp() < 0) {
		perror("Failed to lockItUp");
	}
}

static void signalHandler(int signo, siginfo_t *si, void *unused) {
	// get time
	unsigned long curr_time;
	rdtscll(curr_time);
	unsigned long parent_time = (unsigned long)si->si_value.sival_ptr;
	//printf("Signal time: %ld\tParent time: %ld\tTime elapsed: %ld\n", curr_time, parent_time, curr_time - parent_time);
	printf("(%ld)\n", curr_time - parent_time);
}

int main(int argc, char** argv) {
	initProcess();

	pid_t child_pid = fork();
	if (child_pid == -1) {
		perror("First fork failed");
		return -1;
	} else if (child_pid == 0) {
		initProcess();

		// child process - setup handler. Recv signals and print times
		struct sigaction sa;

		sa.sa_flags = SA_SIGINFO;
		sigemptyset(&sa.sa_mask);
		sa.sa_sigaction = signalHandler;
		if (sigaction(SIGRTMIN + 1, &sa, NULL) == -1) {
			perror("Failed to register the restart handler");
			return -1;
		}
		while(1) {};
	} else {
		// parent process - sleep, then signal
		while (1) {
			usleep(100000);
			// get time
			unsigned long curr_time;
			rdtscll(curr_time);
			union sigval time_value;
			time_value.sival_ptr = (void *)curr_time;
			sigqueue(child_pid, SIGRTMIN + 1, time_value);
		}
	}
}