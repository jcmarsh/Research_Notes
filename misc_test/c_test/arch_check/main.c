#include <stdio.h>

int main (int argc, char** argv) {
#if defined(__x86_64__)
  printf("x86_64\n");
#elif defined(__ARM_ARCH_6__)
  printf("Arm 6\n");
#elif defined(__ARM_ARCH_7__)
  printf("Arm 7\n");
#elif defined(__ARM_ARCH_7A__)
  printf("Arm 7A\n");
#else
  printf("Unknown\n");
#endif

  return 1;
}
