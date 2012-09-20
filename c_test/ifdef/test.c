#include <stdio.h>

void main() {
#ifdef VALUE
  printf("VALUE was defined\n");
#else
  printf("VALUE was NOT defined\n");
#endif
}
