#include <stdio.h>

int main(int argc, char** argv) {
  int x = 4;
  int y = 9;
  int *z;

  *z = y;

  printf("Aught to be 9: %d\n", *z);
}
