/*
 * Example program with a select loop and timeouts
 *
 * James Marshall
 */

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define SIG SIGUSR1

// restart timer fd
char timeout_byte[1] = {'*'};
int timeout_fd[2];
timer_t timerid;
struct itimerspec its;

// Signal handler setup by ????
void timeout_sighandler(int signum) {
  // Write a single byte to the timeout pipe
  // This will detected in the select call
  write(timeout_fd[1], timeout_byte, 1);
}

int initTimer() {
  struct sigevent sev;
  struct sigaction sa;
  sigset_t mask;

  // Setup the timeout pipe
  if (pipe(timeout_fd) == -1) {
    perror("Pipe create fail");
    return -1;
  }

  // Setup the signal handler
  if (signal(SIG, timeout_sighandler) == SIG_ERR) {
    perror("sigaction failed");
    return -1;
  }

  // Make sure that the timeout signal isn't blocked (will be by default).
  sigemptyset(&mask);
  sigaddset(&mask, SIG);
  if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
    perror("sigprockmask failed");
    return -1;
  }

  // Create the timer
  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIG;
  sev.sigev_value.sival_ptr = &timerid;
  if (timer_create(CLOCK_MONOTONIC, &sev, &timerid) == -1) {
    perror("timer_create failed");
    return -1;
  }

  return 0;
}

int main(int argc, const char **argv) {
  if (initTimer() < 0) {
    puts("ERROR: initTimer failed.\n");
    return -1;
  }

  // Arm timer
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 0;
  its.it_value.tv_sec = 2;
  its.it_value.tv_nsec = 0;

  if (timer_settime(timerid, 0, &its, NULL) == -1) {
    perror("timer_settime failed");
  }

  while(1) {
    int retval = 0;

    struct timeval select_timeout;
    fd_set select_set;

    // Set the timeout for the select call
    select_timeout.tv_sec = 1;
    select_timeout.tv_usec = 0;

    // The fd_set should have the fd for each source you want to check
    FD_ZERO(&select_set);
    // Check for timeouts
    FD_SET(timeout_fd[0], &select_set);

    // The Comm Server will need at least one tcp/ip connection with the Robot Interface Server
    // for receiving updates from the simulator
    // FD_SET(sim_server_fd, &select_set);

    // The Comm Server will also need to communicate with the control program (started by plumber)
    // When plumber is launched, Comm Server will pass through the write side to a pipe. The read
    // side of that pipe should be checked here for commands from the control program.
    // FD_SET(control_fd, &select_set);

    // This will wait at least timeout until return. Returns earlier if something has data.
    retval = select(FD_SETSIZE, &select_set, NULL, NULL, &select_timeout);

    if (retval > 0) {
      // One of the fds has data to read
      // Figure out with one with FD_ISSET

      // Check for failed replica (time out)
      if (FD_ISSET(timeout_fd[0], &select_set)) {
        char theByte = 'a';
        // Don't forget to read the character to unset select
        read(timeout_fd[0], &theByte, sizeof(char));

        printf("Timeout expired: %c\n", theByte); // What is the answer? Print as %d.

        // rearm the timer
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;
        its.it_value.tv_sec = 2;
        its.it_value.tv_nsec = 0;

        if (timer_settime(timerid, 0, &its, NULL) == -1) {
          perror("timer_settime failed");
        }
      }

      // Check for data from the Robot Interface Server
      // if (FD_ISSET(sim_server_fd, &select_set)) {
        // These are simulator data that should be written to the control program.
        // You may want to reset the timer here (since the Robot Interface Server is alive).
      // }

      // Check for data from the control program
      // if (FD_ISSET(control_fd, &select_set)) {
        // These are commands from the control program that should be written to the Robot Interface Server.
      // }
    }
  }
}
