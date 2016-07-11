#include "tas_time.h"

//-----------------------------------------------------------------------------
/// Wraps the rdtsc assembly call and macro into a C/C++ function.
/// @return the current timestamp as indicated by the rdtsc register.
#if defined(__x86_64__)
timestamp_t generate_timestamp( void ) {
  timestamp_t ts;
  rdtscll( ts );
  return ts;
}
#elif defined(__ARM_ARCH_7A__)
timestamp_t generate_timestamp( void ) {
  unsigned int pmccntr;
  unsigned int pmuseren;
  unsigned int pmcntenset;

  // Read the user mode perf monitor counter access permissions.
  asm volatile ("mrc p15, 0, %0, c9, c14, 0" : "=r" (pmuseren));
  if (pmuseren & 1) {  // Allows reading perfmon counters for user mode code.
    asm volatile ("mrc p15, 0, %0, c9, c12, 1" : "=r" (pmcntenset));
    if (pmcntenset & 0x80000000ul) {  // Is it counting?
      asm volatile ("mrc p15, 0, %0, c9, c13, 0" : "=r" (pmccntr));
      // The counter is set up to count every 64th cycle
      return (((timestamp_t) pmccntr) * 64);  // Should optimize to << 6
    }
  }
  //  printf("Arm performance monitor not enabled (check for module)\n");
  return 0;
}
#else
timestamp_t generate_timestamp( void ) {
  //  printf("Unsupported Arch for timestamps!\n");
  return 0;
}
#endif

float diff_time(timestamp_t current, timestamp_t last, float cpu_mhz) {
  if (current > last) {
    return (current - last) / cpu_mhz;
  } else {
#if defined(__ARM_ARCH_7A__)
    return ((((timestamp_t) UINT_MAX * 64) - last) + current) / cpu_mhz;
#else
    return 0.0;
#endif
  }
}
