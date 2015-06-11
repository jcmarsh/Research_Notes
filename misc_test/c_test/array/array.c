#include <stdio.h>
#include <unistd.h>

void main() {
  int len =  16;
  double buff[2];
  int i;
  void * buf_p;

  buff[0] = .1;
  buff[1] = .2;

  buf_p = &buff;
  for (i = 0; i < len; i++) {
    printf("%x ", ((unsigned char *)buf_p)[i]);
  }
  printf("\n");
}
  
/*
void main() {
  int len = 10;
  char buffer[len];
  int i;
  void * buf;

  buffer[0] = 'H';
  buffer[1] = 'e';
  buffer[2] = 'l';
  buffer[3] = 'l';
  buffer[4] = '0';
  buffer[5] = ' ';
  buffer[6] = 'J';
  buffer[7] = 'a';
  buffer[8] = 'm';
  buffer[9] = 'e';

  for (i = 0; i < len; i++) {
    printf("%2x ", buffer[i]);
  }

  buf = &buffer;

  printf("\n");

  for (i = 0; i < len; i++) {
    printf("%2x ", ((char *)buf)[i]);
  }
  printf("\n");
}
*/
