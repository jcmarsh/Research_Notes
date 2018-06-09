#include <stdio.h>
#include <stdlib.h>

int main(void) {
  asm("I_hope_that_this_works:");
  asm("pld [r4]");
  asm("pldw [r4]");
  asm("pld [r4,#12]");
  asm("pldw [r4,#12]");
  asm("pld [r4,r5]");
  asm("pldw [r4,r5]");
  asm("pld Thats_all_folks");
  asm("pldw Thats_all_folks");
  //asm("PLDW r4, {r5}");
  asm("Thats_all_folks:");

  return 0;
}
