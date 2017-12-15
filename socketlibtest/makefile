.PHONY:clean all

WORKDIR=.


CC=gcc
CFLGS= -Wall -g -I$(WORKDIR)/
LIBFLAG = -L$(HOME)/lib

BIN=myserver myclient  

all:$(BIN)
	

	
myserver:myserver.o
	$(CC) $(LIBFLAG) -lpthread -litcastsocket $^ -o $@ 

myclient:myclient.o
	$(CC) $(LIBFLAG) -g -lpthread -litcastsocket $^ -o $@ 

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@	

clean:
	rm -f *.o $(BIN)




