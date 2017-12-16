.PHONY:clean all

WORKDIR=.
VPATH = ./src

CC=gcc
CFLGS= -Wall -g -I$(WORKDIR)/inc/
LIBFLAG = -L$(HOME)/lib


BIN = testdbapi

all:$(BIN)
		
testdbapi:testdbapi.o  
	$(CC) $(LIBFLAG) $^ -o $@ -lpthread  -lclntsh  -licdbapi
%.o:%.c
	$(CC) $(CFLGS) -c $< -o $@	

clean:
	rm -f *.o $(BIN)
	
	
	




