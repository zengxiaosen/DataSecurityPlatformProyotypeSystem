.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=dm01_writeshm dm02_readshm dm03_shmop dm04_shmop

all:$(BIN)
%.o:%.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(BIN)