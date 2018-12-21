// Sends a message...

#include "zhelpers.h"

int main(void) {
  void *context = zmq_ctx_new();

  // set up sender to send messages to the sb
  void *sender = zmq_socket(context, ZMQ_PUSH);
  zmq_connect(sender, "ipc:///tmp/zero_sb_in");
  printf("PI: Connected to SB to send messages\n");

  // set up subscriber to get new messages from subscribed MIDs
  void *subscriber = zmq_socket(context, ZMQ_SUB);
  int rc = zmq_connect(subscriber, "ipc:///tmp/zero_sb_pub");
  assert(rc == 0);
  printf("PI: Connected to SB to receive messages\n");

  char *filter = "11";
  rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, filter, strlen(filter));
  assert(rc == 0);
  printf("PI: \tAnd the filter is in place.\n");

  // Ping should send a message...
  zmq_send(sender, "10 ping", 7, 0);

  // And wait for pong to respond
  char buffer[256];
  rc = zmq_recv(subscriber, buffer, 256, 0);
  if (rc > 255) {
    printf("PI: I can't handle this\n");
  } else {
    buffer[rc] = '\0';
    printf("PI: Received: %s\n", buffer);
  }
  
  zmq_close(sender);
  zmq_close(subscriber);
  zmq_ctx_destroy(context);
  return 0;
}
