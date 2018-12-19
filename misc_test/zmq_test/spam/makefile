all: client publisher

client: spam_client.c
	gcc spam_client.c -o client -I../../libzmq/include/ -L../../libzmq/src/.libs -lzmq


publisher: spam_publisher.c
	gcc spam_publisher.c -o publisher -I../../libzmq/include/ -L../../libzmq/src/.libs -lzmq

clean:
	rm -f publisher

