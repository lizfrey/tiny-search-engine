# Makefile for Querier
# 	builds executable querier
#
# Elizabeth Frey
# Winter 2021
# CS 50 Lab 6

PROG = querier fuzzquery
OBJS = querier.o ../libcs50/hashtable.o ../libcs50/bag.o ../libcs50/webpage.o ../libcs50/jhash.o ../common/word.o ../common/index.o
OBJS += ../libcs50/set.o ../libcs50/memory.o ../common/pagedir.o ../libcs50/file.o ../libcs50/counters.o
LIBS = 
CFLAGS =  -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common

all: $(PROG)

querier: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

fuzzquery: fuzzquery.o ../libcs50/file.o ../libcs50/memory.o 
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


test: $(PROG) testing.sh
	bash -v ./testing.sh
	make clean

# dependences
querier.o: ../libcs50/hashtable.h ../libcs50/bag.h ../libcs50/jhash.h ../common/word.h ../common/index.h ../libcs50/set.h ../libcs50/memory.h ../common/pagedir.h ../libcs50/file.h ../libcs50/counters.h ../libcs50/webpage.h

fuzzquery.o: ../libcs50/file.h ../libcs50/memory.h

.PHONY: clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f core
