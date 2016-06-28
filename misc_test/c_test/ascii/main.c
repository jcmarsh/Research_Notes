#include <stdio.h>

int main (int argc, char** argv) {
  char a = 'a';
  char A = 'A';
  int i = 0;

  printf("Mystery: %c: 0x%02X\n", 0x0A, 0x0A);

  for (i = 0; i < 26; i++) {
    printf("%c: 0x%2X\t %c: 0x%2X\n", a, a, A, A);
    a++;
    A++;
  }

  return 0;
}
