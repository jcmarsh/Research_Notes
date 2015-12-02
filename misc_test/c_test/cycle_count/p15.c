// Found code: http://gperftools.googlecode.com/svn/trunk/src/base/cycleclock.h
// Another reference: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0290g/Bihcgfcf.html 

#include <stdio.h>
#include <stdint.h>

// This should return the number of cycles since power-on.  Thread-safe.
static inline int64_t Now() {
	uint32_t pmccntr;
	uint32_t pmuseren;
	uint32_t pmcntenset;
	// Read the user mode perf monitor counter access permissions.
	asm volatile ("mrc p15, 0, %0, c9, c14, 0" : "=r" (pmuseren));
	if (pmuseren & 1) {  // Allows reading perfmon counters for user mode code.
		asm volatile ("mrc p15, 0, %0, c9, c12, 1" : "=r" (pmcntenset));
		if (pmcntenset & 0x80000000ul) {  // Is it counting?
			asm volatile ("mrc p15, 0, %0, c9, c13, 0" : "=r" (pmccntr));
			// The counter is set up to count every 64th cycle
			return (int64_t)((pmccntr) * 64);  // Should optimize to << 6
		}
	} else {
	  printf("Can't read from user mode\n");
	}
}

int main(int argc, char* argv[]) {
	printf("Test program for cycle count on ARM using p15\n");
	int64_t current, last;
	int i, sum = 0;
	int max = 10000;

	if (argc > 1) {
	  max = atoi(argv[1]);
	}

	last = Now();
	for (i = 0; i < max; i++) {
		// loop to take some time.
		sum += i;
	}
	current = Now();

	printf("Looped %d times in %lld cycles.\n", max, current - last);

	return 0;
}
