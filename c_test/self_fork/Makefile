RESEARCH=/home/jcmarsh/research
PLAYERC=$(RESEARCH)/player
INC=-I$(PLAYERC)/ -I$(PLAYERC)/client_libs/ -I$(PLAYERC)/build/

all: regular

regular:
	gcc $(INC) -o forked_self self_forked.c -lplayerc -lrt
	gcc $(INC) -o control main.c

clean:
	rm -f forked_self
	rm -f control

