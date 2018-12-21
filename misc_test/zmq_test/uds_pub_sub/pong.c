// Waits for a ping, sends a pong

#include "zhelpers.h"

int main(void) {
  void *context = zmq_ctx_new();

  // set up sender to send messages to the sb
  void *sender = zmq_socket(context, ZMQ_PUSH);
  zmq_connect(sender, "ipc:///tmp/zero_sb_in");
  printf("PO: Connected to SB to send messages\n");

  // set up subscriber to get new messages from subscribed MIDs
  void *subscriber = zmq_socket(context, ZMQ_SUB);
  int rc = zmq_connect(subscriber, "ipc:///tmp/zero_sb_pub");
  assert(rc == 0);
  printf("PO: Connected to SB to receive messages\n");

  char *filter = "10";
  rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, filter, strlen(filter));
  assert(rc == 0);
  printf("PO: \tAnd the filter is in place.\n");

  // Wait for a ping
  char buffer[256];
  rc = zmq_recv(subscriber, buffer, 256, 0);
  if (rc > 255) {
    printf("PO: I can't handle this\n");
  } else {
    buffer[rc] = '\0';
    printf("PO: Received: %s\n", buffer);
  }

  // Respond with a pong
  zmq_send(sender, "11 pong", 7, 0);
  
  zmq_close(sender);
  zmq_close(subscriber);
  zmq_ctx_destroy(context);
  return 0;
}
