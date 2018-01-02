all:
	gcc wrap.c -c -o wrap.o
	gcc client.c wrap.o -o client
	gcc server.c wrap.o -o server
.PHONY:clean
clean:
	rm wrap.o
	rm server
	rm client
