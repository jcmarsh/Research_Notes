LIB=-lrt

%.o: %.c
	gcc -c -o $@ $<

all: First Second

First: first.o
	gcc -o First first.o $(LIB)

Second: second.o
	gcc -o Second second.o $(LIB)

clean:
	rm -f First
	rm -f Second
	rm -f *.o
