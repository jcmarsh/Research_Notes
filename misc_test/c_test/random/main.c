// Trying to test out random number generation along with LZO's types

#include <stdio.h>
#include <stdlib.h>

#define __LZO_MMODEL
#define IN_LEN      (64*1024ul)

static unsigned char __LZO_MMODEL in  [ IN_LEN ];

int main(int argc, char *argv[]) {
  unsigned int in_len;
  void * sigh;
  
  printf("Let's see how this all goes...\n");

  in_len = IN_LEN;
  for (int i = 0; i < in_len; i++) {
    in[i] = rand() % 256;
  }

  printf("Setting input to random: 0x%02X 0x%02X 0x%02X 0x%02X\n", in[0], in[1], in[2], in[3]);
  
  sigh = &(in[0]);
  printf("Ermmm... let's see: 0x%08X\n", *(int *)(sigh));
  sigh = &(in[1]);
  printf("Ermmm... let's see: 0x%08X\n", *(int *)(sigh));
  sigh = &(in[2]);
  printf("Ermmm... let's see: 0x%08X\n", *(int *)(sigh));
  sigh = &(in[3]);
  printf("Ermmm... let's see: 0x%08X\n", *(int *)(sigh));

}
  
