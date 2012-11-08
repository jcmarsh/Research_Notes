#include <stdio.h>
#include <unistd.h>

void main() {
  unsigned long mask = (unsigned long)1 << 63;
  unsigned long value = 0x8000000011112222;

  printf("Mask: \t%lx\n", mask);
  printf("Not Mask: \t%lx\n", ~mask);
  printf("Value: \t%lx\n", value);
  printf("Masked Val: \t%lx\n", value & (~mask));
}
