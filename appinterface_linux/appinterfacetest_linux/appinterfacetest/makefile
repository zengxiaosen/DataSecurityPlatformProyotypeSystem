# Makefile 
.SUFFIXES:
.SUFFIXES:  .c .o

all:appinterfacetest


CC	= gcc


WORKDIR=.
LIBDIR=$(HOME)/lib
LIBS = -L$(LIBDIR) -lappinterface
OUTDIR=$(HOME)/bin

INCLDIR=$(WORKDIR)
INCLFLG= -I$(WORKDIR)
CFLAG= $(INCLFLG) -g -c -Wall

VPATH = $(WORKDIR)

OBJ1 = appcryptapitest.o 

appinterfacetest:$(OBJ1)
	$(CC) $(LIBS) $^ -o $@ 

.c.o:
	$(CC) $(CFLAG) $<

.PHONY:clean
clean:
	rm *.o
	rm testmsgreal
	rm *.ber
	rm *.txt

