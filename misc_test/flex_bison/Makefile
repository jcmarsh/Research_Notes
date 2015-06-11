pb: pb.l pb.y
	bison -d pb.y
	flex pb.l
	cc pb.tab.c lex.yy.c -lfl
