#include <stdio.h>
#include <unistd.h>

// I want to test the behavior of casting from large to small data types

void main() {
  unsigned char array[6] = {0};
  unsigned int trouble = 0xFAFBFCFD;
  int index;

  array[1] = trouble;

  printf("Printing the array:\n");
  for (index = 0; index < 6; index++) {
    printf("0x%02X\t", array[index]);
  }
  printf("\n");
}
