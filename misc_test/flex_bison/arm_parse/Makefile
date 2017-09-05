default: arm.l arm.y
	bison -d arm.y
	flex arm.l
	cc arm.tab.c lex.yy.c -lfl

clean:
	rm -f arm.tab.c
	rm -f arm.tab.h
	rm -f lex.yy.c
