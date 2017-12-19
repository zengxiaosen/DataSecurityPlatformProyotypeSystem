.PHONY:clean all

WORKDIR=.
VPATH = ./src

CC=gcc
CFLGS= -Wall -g -I$(WORKDIR)/inc/
LIBFLAG = -L$(HOME)/lib


BIN = keymngclient  keymngserver 
#testdbapi

all:$(BIN)

#  myipc_shm.o  keymng_shmop.o keymng_dbop.o
keymngserver:keymngserver.o  keymngserverop.o  keymnglog.o
	$(CC) $(LIBFLAG) $^ -o $@ -lpthread -litcastsocket -lmessagereal  -lclntsh  -licdbapi



#myipc_shm.o keymng_shmop.o
keymngclient:keymngclient.o  keymnglog.o keymngclientop.o
	$(CC) $(LIBFLAG) -lpthread -litcastsocket -lmessagereal $^ -o $@ 




#testdbapi:testdbapi.o  
#	$(CC) $(LIBFLAG) $^ -o $@ -lpthread  -lclntsh  -licdbapi
		
%.o:%.c
	$(CC) $(CFLGS) -c $< -o $@	

clean:
	rm -f *.o $(BIN)
	
	
	




