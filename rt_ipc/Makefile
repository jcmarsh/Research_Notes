all: PipeTest

%.o: %.c
	gcc -c -Werror -o $@ $<

PipeTest: pipe_test.o
	gcc -o PipeTest pipe_test.o -lrt

clean:
	rm -f *.o
	rm -f PipeTest
