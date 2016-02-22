/*-----------------------------------------------------------------------------
time.h encapsulates all time operations including API types and queries and 
POSIX types and queries.

author: James Taylor : jrt@gwu.edu
-----------------------------------------------------------------------------*/

#ifndef _TIME_H_
#define _TIME_H_

//-----------------------------------------------------------------------------

#include <sys/time.h>           // POSIX time
#include <limits.h>

//-----------------------------------------------------------------------------

/// timestamp type in cycles.
typedef unsigned long long timestamp_t;

//-----------------------------------------------------------------------------

/// Assembly call to get current content of rdtsc register
//#define rdtscll(val) __asm__ __volatile__("rdtsc" : "=A" (val))
#define rdtscll(value)				       \
  __asm__ ("rdtsc\n\t"				       \
	   "shl $(32), %%rdx\n\t"		       \
	   "or %%rax, %%rdx" : "=d" (value) : : "rax")


//-----------------------------------------------------------------------------

timestamp_t generate_timestamp( void );

float diff_time(timestamp_t current, timestamp_t last, float cpu_mhz);

#endif // _TIME_H_
