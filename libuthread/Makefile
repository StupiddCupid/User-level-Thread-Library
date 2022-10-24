# Target library
lib := libuthread.a
objs	:= libuthread.o
CC		:= gcc
CFLAGS 	:= -Wall -Wextra -Werror 
CFLAGS 	+= -g



all: $(lib)

## TODO: Phase 1
libuthread.a:
ar rcs libuthread.a libuthread.o

libuthread.o: libuthread/queue.c libuthread/queue.h
	$(CC) $(CFLAGS)  libuthread/queue.c -o libuthread/queue

clean:
	rm -f $(objs) $(lib)
