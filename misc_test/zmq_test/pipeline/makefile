INC = -I../../libzmq/include -I../common_inc/
LIB = -L../../libzmq/src/.libs -lzmq

all: ventilator worker sink

ventilator: ventilator.c
	gcc ventilator.c -o ventilator $(INC) $(LIB)

worker: worker.c
	gcc worker.c -o worker $(INC) $(LIB)

sink: sink.c
	gcc sink.c -o sink $(INC) $(LIB)

clean:
	rm -f ventilator
	rm -f worker
	rm -f sink
