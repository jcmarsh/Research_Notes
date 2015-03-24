all: TimerTest

%.o: %.c
	gcc -c -Werror -o $@ $<

TimerTest: timer_test.o
	gcc -o TimerTest timer_test.o -lrt

clean:
	rm -f *.o
	rm -f TimerTest
