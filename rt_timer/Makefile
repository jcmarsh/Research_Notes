all: TimerTest SignalTest

%.o: %.c
	gcc -c -Werror -o $@ $<

TimerTest: timer_test.o tas_time.c tas_time.h
	gcc -o TimerTest timer_test.o tas_time.c -lrt

SignalTest: signal_test.o tas_time.c tas_time.h
	gcc -o SignalTest signal_test.o tas_time.c -lrt

clean:
	rm -f *.o
	rm -f TimerTest
	rm -f SignalTest
