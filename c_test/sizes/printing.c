#include <stdio.h>

int main(int argc, char** argv) {
  long test = 1024 * 1024;
  test = test * 1024 * 1024; // 2^10 * 2^10 * 2^10 * 2^10 = 2^40

  printf("Long printed as int:\t %d\n", test);
  printf("Long printed as long:\t %ld\n", test);

  return 0;
}
