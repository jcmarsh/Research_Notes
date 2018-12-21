INC = -I../../libzmq/include -I../common_inc/
LIB = -L../../libzmq/src/.libs -lzmq

all: client publisher

client: spam_client.c
	gcc spam_client.c -o client $(INC) $(LIB)


publisher: spam_publisher.c
	gcc spam_publisher.c -o publisher $(INC) $(LIB)

clean:
	rm -f client
	rm -f publisher
