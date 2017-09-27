default: bench encrypt decrypt

encrypt: encrypt.c rijndael.c rijndael.h
	gcc -o encrypt encrypt.c rijndael.c

decrypt: decrypt.c rijndael.c rijndael.h
	gcc -o decrypt decrypt.c rijndael.c

bench: bench_aes.c rijndael.c rijndael.h
	gcc -g -o bench bench_aes.c rijndael.c

clean:
	rm -f encrypt
	rm -f decrypt
	rm -f bench
