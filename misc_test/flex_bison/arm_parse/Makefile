parser: arm.l arm.y
	bison -d arm.y
	flex arm.l
	cc -o arm_parser arm.tab.c lex.yy.c -lfl

clean:
	rm -f arm_parser
	rm -f arm.tab.c
	rm -f arm.tab.h
	rm -f lex.yy.c
