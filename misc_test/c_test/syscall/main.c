#include <pthread.h>
#include <sched.h>

int main(int argc, char **argv) {
  /*
  // Apparently this is the wrong way...
  struct sched_param param;
  param.sched_priority = 50;
  // set the scheduler as with policy of round robin (realtime)
  if( sched_setscheduler(getpid(), SCHED_RR, &param ) == -1 ) {
    perror("sched_setscheduler SCHED_RR ");
    return -1;
  }
  */

  struct sched_param param;
  param.sched_priority = 45;
  if (pthread_setschedparam(pthread_self(), SCHED_RR, &param) < 0) {
    puts("pthread_setschedparam error\n");
    return -1;
  }

  while (1) {

  }

  return 0;
}
