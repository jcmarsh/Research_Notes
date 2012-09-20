#include <stdio.h>
#include "foo.h"

void hello(void) {
  printf("Hello James\n");
}

void hello_arg(unsigned long a) {
  printf("Number: %lx\n", a);
}
