parser: arm_openocd.l arm_openocd.y
	bison -d arm_openocd.y
	flex arm_openocd.l
	cc -g -o arm_parser_openocd arm_openocd.tab.c lex.yy.c -lfl
	gcc -c -fpic arm_openocd.tab.c lex.yy.c -lfl
	gcc -shared -o libarmparse_openocd.so arm_openocd.tab.o lex.yy.o -lfl

lib_test:
	gcc -o lib_test lib_test.c -larmparse_openocd -lfl -L./

install: libarmparse_openocd.so
	cp libarmparse_openocd.so /usr/lib

clean:
	rm -f *.o
	rm -f libarmparse_openocd.so
	rm -f arm_parser_openocd
	rm -f arm_openocd.tab.c
	rm -f arm_openocd.tab.h
	rm -f lex.yy.c
	rm -f lib_test
