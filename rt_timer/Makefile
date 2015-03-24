all: TimerTest SignalTest

%.o: %.c
	gcc -c -Werror -o $@ $<

TimerTest: timer_test.o
	gcc -o TimerTest timer_test.o -lrt

SignalTest: signal_test.o
	gcc -o SignalTest signal_test.o -lrt

clean:
	rm -f *.o
	rm -f TimerTest
	rm -f SignalTest
