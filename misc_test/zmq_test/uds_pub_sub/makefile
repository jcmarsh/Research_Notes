INC = -I../../libzmq/include -I../common_inc/
LIB = -L../../libzmq/src/.libs -lzmq

all: zero_sb ping pong

ping: ping.c
	gcc ping.c -o ping $(INC) $(LIB)

pong: pong.c
	gcc pong.c -o pong $(INC) $(LIB)

zero_sb: zero_sb.c
	gcc zero_sb.c -o zero_sb $(INC) $(LIB)

clean:
	rm -f ping
	rm -f pong
	rm -f zero_sb
