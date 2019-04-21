DEP=flat_inc/*.h
INC=-Iflat_inc -Iflatcc/include
LIB=-Lflatcc/lib -lflatcc
FLAGS=-g

all: writer reader

writer: writer.c ${DEP}
	gcc -o writer ${FLAGS} writer.c ${INC} ${LIB}

reader: reader.c ${DEP}
	gcc -o reader ${FLAGS} reader.c ${INC} ${LIB}

clean:
	rm -f reader
	rm -f writer
	rm -f evs_msg
