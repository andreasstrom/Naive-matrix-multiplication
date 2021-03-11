LD = gcc
CFLAGS = -Werror -lm -std=c99 -g 
VFLAGS = -msse4.1 $(CFLAGS)
PFLAGS = $(CFLAGS) -pthread 
OFLAGS = $(PFLAGS) -O2 -march=native
GPFLAGS = -Wall -lm -std=c99 -pg 
OBJS = naive_main.c naive_help.c
PNAME = naive
LIBS = -lm -lpthread

$(PNAME): naive_algorithm.o naive_help.o naive_help.h
	$(LD) -o $@ $^ $(LIBS)

naive_algorithm.o: naive_algorithm.c naive_help.h
	$(LD)  $(PFLAGS) -c -o $@  $<

naive_help.o: naive_help.c naive_help.h
	$(LD) $(PFLAGS) -c -o $@  $<

naive_algorithm.c: naive_help.h

naive_help.c: naive_help.h

clean:
	rm *.o $(PNAME)
