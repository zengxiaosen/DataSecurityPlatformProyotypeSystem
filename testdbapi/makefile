.PHONY:clean all

WORKDIR=.
VPATH = ./src

CC=gcc
CFLGS= -Wall -g -I$(WORKDIR)/inc/
LIBFLAG = -L$(HOME)/lib


BIN = testdbapi testdbapi2

all:$(BIN)
		
testdbapi:testdbapi.o  
	$(CC) $(LIBFLAG) $^ -o $@ -lpthread  -lclntsh  -licdbapi

testdbapi2:testdbapi2.o  
	$(CC) $(LIBFLAG) $^ -o $@ -lpthread  -lclntsh  -licdbapi

%.o:%.c
	$(CC) $(CFLGS) -c $< -o $@	

clean:
	rm -f *.o $(BIN)
	
	
	




