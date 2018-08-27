#include <stdint.h>
#include <stdio.h>

void SwapUInt64(uint64_t *ValueToSwap)
{
  uint8_t *BytePtr = (uint8_t *)ValueToSwap;
  uint8_t  TempByte;

  TempByte  = BytePtr[7];
  BytePtr[7] = BytePtr[0];
  BytePtr[0] = TempByte;

  TempByte   = BytePtr[6];
  BytePtr[6] = BytePtr[1];
  BytePtr[1] = TempByte;

  TempByte   = BytePtr[5];
  BytePtr[5] = BytePtr[2];
  BytePtr[2] = TempByte;

  TempByte   = BytePtr[4];
  BytePtr[4] = BytePtr[3];
  BytePtr[3] = TempByte;
}

void main(int argc, void * argv) {
  uint64_t original = 0x0102030405060708;
  uint64_t reversed = original;

  void * original_pointer = &original;
  void * reversed_pointer = original_pointer;

  printf("What's up?\n");
  SwapUInt64(&reversed);
  printf("Reverse 0x%016lX <-> 0x%016lX\n", original, reversed);

  SwapUInt64((void *)&reversed_pointer);
  printf("Reverse Pointer 0x%016lX <-> 0x%016lX\n", original_pointer, reversed_pointer);
}
