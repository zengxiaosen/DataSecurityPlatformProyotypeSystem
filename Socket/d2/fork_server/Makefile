all:
	gcc wrap.c -c
	gcc server.c -c
	gcc client.c -c
	gcc wrap.o server.o -o server
	gcc wrap.o client.o -o client

.PHONY:clean

clean:
	rm server client *.o
