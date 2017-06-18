#include <stdio.h>
#include <unistd.h>

int gl_i = 0;

void main() {
  int loop = 10;
  int i;
  
  for (i = 0; gl_i < loop; gl_i++) {
    printf("What's the word? \%d\n", gl_i);
  }
}
