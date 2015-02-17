#define _GNU_SOURCE

#include <sched.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

#define ORPHAN_TO_ANCESTOR (SIGRTMIN + 1)
#define ANCESTOR_TO_ORPHAN (SIGRTMIN + 2)

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

	unsigned long last_time;
	rdtscll(last_time);
	
	if (lockItUp() < 0) {
		perror("Failed to lockItUp");
	}
	
	unsigned long curr_time;
	rdtscll(curr_time);
	printf("(%ld)\n", curr_time - last_time);
}

static void signalHandler(int signo, siginfo_t *si, void *unused) {
	// get time
	//unsigned long curr_time;
	//rdtscll(curr_time);
	//unsigned long parent_time = (unsigned long)si->si_value.sival_ptr;
	//printf("Signal time: %ld\tParent time: %ld\tTime elapsed: %ld\n", curr_time, parent_time, curr_time - parent_time);
	//printf("(%ld)\n", curr_time - parent_time);
}

pid_t child_pid;
pid_t ancestor_pid;

static void parentHandler(int signo, siginfo_t *si, void *unused) {
	// get sender pid
	child_pid = si->si_pid;
	printf("New child pid set to: %d\n", child_pid);
}

int child_counter = 10;

void fork_n_die(void) {
	initProcess();

	// child process, create orphan
	child_pid = fork();
	if (child_pid == -1) {
		perror("fork_n_die fork failed");
		return;
	} else if (child_pid == 0) {
		child_counter--;
		if (child_counter > 0) {
			fork_n_die();
		} else {
			initProcess();

			printf("Soon to be Orphan here, signal to %d\n", ancestor_pid);
			// child process
			// setup handler first so will not terminate when signal recieved.
			struct sigaction sa;
			sa.sa_flags = SA_SIGINFO;
			sigemptyset(&sa.sa_mask);
			sa.sa_sigaction = signalHandler;
			if (sigaction(ANCESTOR_TO_ORPHAN, &sa, NULL) == -1) {
				perror("Failed to register the child handler");
				return;
			}
			// send signal (so ancestor has orphan pid)
			if (kill(ancestor_pid, ORPHAN_TO_ANCESTOR) < 0) {
				perror("Orphan failed to signal ancestor");
			}

			while(1) {}
		}
	} else {
		//printf("Red Parent is about to die. %d and %d\n", ancestor_pid, getpid());
		// parent - sleep to give kid a chance, then exit
		//sleep(1);
		exit(0);
	}
}

int main(int argc, char** argv) {
	initProcess();

	ancestor_pid = getpid();
	child_pid = fork();

	if (child_pid == -1) {
		perror("First fork failed");
		return -1;
	} else if (child_pid == 0) {
		fork_n_die();	
	} else {
		printf("The Ancestor runs: %d\n", getpid());
		struct sigaction sa;

		sa.sa_flags = SA_SIGINFO;
		sigemptyset(&sa.sa_mask);
		sa.sa_sigaction = parentHandler;
		if (sigaction(ORPHAN_TO_ANCESTOR, &sa, NULL) == -1) {
			perror("Failed to register the parent handler");
			return -1;
		}

		while (1) {
			usleep(100000);
			// get time
			//unsigned long curr_time;
			//rdtscll(curr_time);
			//union sigval time_value;
			//time_value.sival_ptr = (void *)curr_time;
			//sigqueue(child_pid, ANCESTOR_TO_ORPHAN, time_value);
		}
	}
}
