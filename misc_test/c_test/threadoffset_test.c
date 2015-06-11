#include <stdio.h>
#include <stddef.h>
#include "../../../../kernel/include/thread.h"

void main(void) {
  printf("Offset of thread regs: %zd\n", offsetof(struct thread, regs));
}
