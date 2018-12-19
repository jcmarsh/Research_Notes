#include "zhelpers.h"

int main(void) {
  void *context = zmq_ctx_new();
  void *publisher = zmq_socket(context, ZMQ_PUB);
  int rc = zmq_bind(publisher, "tcp://*:5556");
  assert(rc == 0);

  srandom((unsigned) time (NULL));
  while(1) {
    int zipcode, temperature, relhumidity;
    zipcode = randof(100000);
    temperature = randof(215) - 80;
    relhumidity = randof(50) + 10;

    char update[20];
    sprintf(update, "%05d %d %d", zipcode, temperature, relhumidity);
    s_send(publisher, update);
  }
  zmq_close(publisher);
  zmq_ctx_destroy(context);
  return 0;
}
