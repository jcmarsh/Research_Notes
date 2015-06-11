RESEARCH=/home/jcmarsh/research
LIBCDIR=$(RESEARCH)/dietlibc-0.33/bin-i386

default: diet

diet:
	diet gcc -nostdinc -o socket_client client.c $(LIBCDIR)/dietlibc.a
# $(SPLAYERC)/libplayerc.a $(LIBCDIR)/dietlibc.a $(LIBCDIR)/librpc.a $(LIBCDIR)/libm.a

normal:
	gcc -o socket_client client.c

clean:
	rm -f socket_client


