// Attempt to recreate some of the SB functionality using ZMQ


#include <zmq.h>

#include <assert.h>
#include <string.h>

#define BUFF_SIZE 4 // initial receive buffer size

int main(void) {
  void *context = zmq_ctx_new();

  // set up publisher to send messages out
  void *publisher = zmq_socket(context, ZMQ_PUB);
  int rc = zmq_bind(publisher, "ipc:///tmp/zero_sb_pub");
  assert(rc == 0);

  // set up receiver to get new messages (to then send out)
  void *receiver = zmq_socket(context, ZMQ_PULL);
  zmq_bind(receiver, "ipc:///tmp/zero_sb_in");

  printf("ZSB: Alright, Zero_SB is up and running!\n");
  while(1) {
    char buffer[BUFF_SIZE];
    rc = zmq_recv(receiver, buffer, BUFF_SIZE, 0);
    assert(rc != -1);
    if (rc > BUFF_SIZE) {
      printf("ZSB: NOT IMPLEMENTED: Message size %d is greater than %d\n", rc, BUFF_SIZE);
      int index = 0;
      char str_buffer[BUFF_SIZE + 1]; // +1 for null
      memcpy(str_buffer, buffer, BUFF_SIZE);
      str_buffer[BUFF_SIZE] = '\0';
      printf("Part %d: %s\n", index, str_buffer);
      rc = zmq_recv(receiver, buffer, BUFF_SIZE, 0);


      buffer[rc] = '\0'; // TODO: Should use a buffer that is BUFF_SIZE + 1 for null
      printf("ZSB: Received: %s\n", buffer);

      // message received, send out?
      int pub_return = zmq_send(publisher, buffer, rc, 0);
      printf("ZSB: pub_return: %d\n", pub_return);


    } else {
      buffer[rc] = '\0'; // TODO: Should use a buffer that is BUFF_SIZE + 1 for null
      printf("ZSB: Received: %s\n", buffer);

      // message received, send out?
      int pub_return = zmq_send(publisher, buffer, rc, 0);
      printf("ZSB: pub_return: %d\n", pub_return);
    }
  }

  zmq_close(publisher);
  zmq_close(receiver);
  zmq_ctx_destroy(context);
  return 0;
}
