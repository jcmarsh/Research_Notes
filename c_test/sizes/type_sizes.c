#include <stdio.h>

int main(int argc, char** argv) {
  printf("char size: \t %zd\n", sizeof(char));
  printf("short size: \t %zd\n", sizeof(short));
  printf("s_int size: \t %zd\n", sizeof(short int));
  printf("int size: \t %zd\n", sizeof(int));
  printf("l_int size: \t %zd\n", sizeof(long int));
  printf("long size: \t %zd\n", sizeof(long));
  printf("l_long size: \t %zd\n", sizeof(long long));
  printf("Pointer size: \t %zd\n", sizeof(void *));

  return 0;
}
