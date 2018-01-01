all:
	gcc -Wall wrap.c -c -o wrap.o
	gcc -Wall client.c wrap.o -o client
	gcc -Wall server.c wrap.o -o server
.PHONY:clean
clean:
	rm wrap.o
	rm server
	rm client
