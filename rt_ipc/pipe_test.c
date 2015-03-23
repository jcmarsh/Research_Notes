#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv) {
	// Need two processes communicating with a pipe.
	// need 3 process; a high priority consumer, a high priority low volume producer, and a low priority high volume producer

	// Make all of the pipes
	int low_pipes[2];
	int high_pipes[2];
	if (pipe(low_pipes) == -1) {
		printf("pipe error\n");
	}
	if (pipe(high_pipes) == -1) {
		printf("pipe error\n");
	}

	pid_t currentPID = fork();

	if (currentPID >= 0) { // Successful fork
		if (currentPID == 0) { // Child process, high prio producer
			// Sched priority high
			struct sched_param param;
			param.sched_priority = 80;
			if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
				perror("Messed up setting non RT scheduler");
			}

			sleep(10);

			while(1) {
				// write to pipe
				char buffer[10] = "High Prio";
				write(high_pipes[1], buffer, sizeof(char) * 10);
				usleep(500000);
			}
		}
	} else {
		printf("Fork error\n");
	}

	currentPID = fork();

	if (currentPID >= 0) { // Successful fork
		if (currentPID == 0) { // Child process, low prio producer
			// Sched priority low
			struct sched_param param;
			param.sched_priority = 70;
			if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
				perror("Messed up setting non RT scheduler");
			}

			sleep(10);

			while(1) {
				char buffer[10] = "Low Prio!";
				write(low_pipes[1], buffer, sizeof(char) * 10);
				int i, j;
				for (i = 0; i < 100000; i++) {
					for (j = 0; j < 10000; j++) {
						int t = i * j;
					}
				}
				usleep(10000);
			}
		}
	} else {
		printf("Fork error\n");
	}

	// parent is the consumer
	// Sched priority high
	struct sched_param param;
	param.sched_priority = 90;
	if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
		perror("Messed up setting non RT scheduler");
	}
	
	struct timeval select_timeout;
	fd_set select_set;

	char in_buffer[10];

	while(1) {
		//printf("What up?\n");
		select_timeout.tv_sec = 1;
		select_timeout.tv_usec = 0;

		FD_ZERO(&select_set);
		FD_SET(high_pipes[0], &select_set);
		FD_SET(low_pipes[0], &select_set);

		int read_ret;
		int retval = select(FD_SETSIZE, &select_set, NULL, NULL, &select_timeout);
		if (retval > 0) {
			if (FD_ISSET(high_pipes[0], &select_set)) {
				read_ret = read(high_pipes[0], in_buffer, sizeof(char) * 10);
				if (read_ret == 10) { printf("High Prio read\n"); }
				else { perror("Read error"); }
			}
			if (FD_ISSET(low_pipes[0], &select_set)) {
				read_ret = read(low_pipes[0], in_buffer, sizeof(char) * 10);
				if (read_ret == 10) { printf("Low Prio read\n"); }
				else { perror("Read error"); }
			}
		}
	}
}