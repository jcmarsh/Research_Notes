#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define RUNS 5000
int main(void) {
  struct timespec start;
  struct timespec end;
  long elapsed_seconds = 0;
  long elapsed_nseconds = 0;

  // Socked to talk to clients
  printf("Connecting to the server\n");
  void *context = zmq_ctx_new();
  void *requester = zmq_socket(context, ZMQ_REQ);
  zmq_connect(requester, "tcp://localhost:5555");

  int request_nbr;
  for (request_nbr = 0; request_nbr < RUNS; request_nbr++) {
    char buffer[10];
    // printf("Sending Hello %d...\n", request_nbr);
    clock_gettime(CLOCK_REALTIME, &start);
    zmq_send(requester, "Hello", 5, 0);
    zmq_recv(requester, buffer, 10, 0);
    clock_gettime(CLOCK_REALTIME, &end);
    // printf("Received World %d\n", request_nbr);

    elapsed_seconds = end.tv_sec - start.tv_sec;
    elapsed_nseconds = end.tv_nsec - start.tv_nsec;
    // printf("Time for function all: %lds and %ldns\n", elapsed_seconds, elapsed_nseconds);
    printf("%ld\n", elapsed_nseconds);
  }
  zmq_close(requester);
  zmq_ctx_destroy(context);

  return 0;
}
