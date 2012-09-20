#include <stdio.h>

void main(void) {
  printf("char size: \t %zd\n", sizeof(char));
  printf("s_int size: \t %zd\n", sizeof(short int));
  printf("int size: \t %zd\n", sizeof(int));
  printf("l_int size: \t %zd\n", sizeof(long int));
  printf("long size: \t %zd\n", sizeof(long));
  printf("l_long size: \t %zd\n", sizeof(long long));
  printf("Pointer size: \t %zd\n", sizeof(void *));
}
