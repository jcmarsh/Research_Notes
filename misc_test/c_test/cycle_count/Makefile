all: arm x86

arm: p15.c
	arm-linux-gnueabi-gcc -o p15_test p15.c

x86: rdtsc.c
	gcc -o rdtsc_test rdtsc.c

clean:
	rm -rf rdtsc_test
	rm -rf p15_test
