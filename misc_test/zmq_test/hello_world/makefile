all: client server

client: hello_client.c
	gcc hello_client.c -o client -I../../libzmq/include/ -L../../libzmq/src/.libs -lzmq

server: hello_server.c
	gcc hello_server.c -o server -I../../libzmq/include/ -L../../libzmq/src/.libs -lzmq

clean:
	rm -f client
	rm -f server
