#include "arm.h"
#include <stdio.h>

int main(int argc, char **argv) {
  char result[200] = {0};
  parse_line(result, "0x00100608	0xe50b3014	STR r3, [r11, #-0x14]\n");
  printf("Result: %s\n", result);

  return 0;
}
