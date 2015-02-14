#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#define rdtscll(value)				       \
  __asm__ ("rdtsc\n\t"				       \
	   "shl $(32), %%rdx\n\t"		       \
	   "or %%rax, %%rdx" : "=d" (value) : : "rax")

static void signalHandler(int signo, siginfo_t *si, void *unused) {
	// get time
	unsigned long curr_time;
	rdtscll(curr_time);
	unsigned long parent_time = (unsigned long)si->si_value.sival_ptr;
	//printf("Signal time: %ld\tParent time: %ld\tTime elapsed: %ld\n", curr_time, parent_time, curr_time - parent_time);
	printf("(%ld)\n", curr_time - parent_time);
}

int main(int argc, char** argv) {
	pid_t child_pid = fork();
	if (child_pid == -1) {
		perror("First fork failed");
		return -1;
	} else if (child_pid == 0) {
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