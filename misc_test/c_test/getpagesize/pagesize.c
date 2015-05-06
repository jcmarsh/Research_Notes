#include <stdio.h>
#include <unistd.h>

void main() {
  printf("Page size: %d\n", getpagesize());
}
