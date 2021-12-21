# Makefile for Common library
# 	Builds and compiles utility functions. 

# object files and the target library
PROG = word index pagedir
OBJS = pagedir.o ../libcs50/memory.o word.o index.o ../libcs50/webpage.o
LIB = common.a

# flags
FLAGS = -I../libcs50

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)


$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# dependences: object files that depend on header files
pagedir.o: pagedir.h ../libcs50/memory.h ../libcs50/webpage.h ../libcs50/file.h
word.o: word.h ../libcs50/memory.h
../libcs50/webpage.o: ../libcs50/webpage.h
index.o: index.h ../libcs50/memory.h ../libcs50/counters.h ../libcs50/hashtable.h ../libcs50/file.h
../libcs50/memory.o: ../libcs50/memory.h

.PHONY: clean

all: $(PROG)

clean:
	rm -f core
	rm -f $(LIB) *~ *.o
	rm -f common.a
	rm -f $(PROG)
