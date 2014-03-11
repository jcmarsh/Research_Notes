default: clean select

detect:
	gcc -o detect -g detect_fault.c

select:
	gcc -Wall -o select -g select.c

fib:
	gcc -o fib -g fib.c utility.h

test:
	gcc -o test -g test.c utility.h

clean:
	rm -f test
	rm -f detect
	rm -f fib
	rm -f select
	rm -f *~
