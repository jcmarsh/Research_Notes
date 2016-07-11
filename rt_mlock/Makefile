all: StressCore MLockTest MLockTest_Busy

%.o: %.c
	gcc -c -Werror -o $@ $<

StressCore: stress_core.o
	gcc -o StressCore stress_core.o -lrt

MLockTest: mlock_test.o tas_time.c tas_time.h force.c force.h
	gcc -o MLockTest mlock_test.o tas_time.c force.c -lrt

MLockTest_Busy: mlock_test.c tas_time.c tas_time.h force.c force.h
	gcc -o MLockTest_Busy mlock_test.c tas_time.c force.c -DPARENT_BUSY -lrt

clean:
	rm -f *.o
	rm -f MLockTest
	rm -f MLockTest_Bus
	rm -f StressCore
