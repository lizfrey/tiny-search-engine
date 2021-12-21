# Makefile for Indexer
# 	builds executable index and indextest file
#
# Elizabeth Frey
# Winter 2021
# CS 50 Lab 4

PROG = indextest indexer
OBJS = ../libcs50/hashtable.o ../libcs50/webpage.o ../libcs50/jhash.o ../common/word.o ../common/index.o
OBJS += ../libcs50/set.o ../libcs50/memory.o ../common/pagedir.o ../libcs50/file.o ../libcs50/counters.o
LIBS = 
CFLAGS =  -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common

all: $(PROG)

indextest: $(OBJS) indextest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer: $(OBJS) indexer.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

test: $(PROG) testing.sh
	bash -v ./testing.sh

# dependences
indexer.o: ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/memory.h ../libcs50/set.h ../libcs50/file.h ../libcs50/jhash.h ../common/pagedir.h ../common/word.h ../common/index.h ../libcs50/counters.h
indextest.o: ../common/index.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/memory.h ../libcs50/set.h ../libcs50/file.h ../libcs50/jhash.h ../common/word.h ../libcs50/counters.h ../common/pagedir.h 

.PHONY: clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	#testing.sh takes care of deleting any files it makes
