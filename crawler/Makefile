# Makefile for Crawler
# 	builds executable crawler file
#
# Elizabeth Frey
# Winter 2021
# CS 50 Lab 4

PROG = crawler
OBJS = crawler.o ../libcs50/bag.o ../libcs50/hashtable.o ../libcs50/webpage.o ../libcs50/jhash.o
OBJS += ../libcs50/set.o ../libcs50/memory.o ../common/pagedir.o ../libcs50/file.o
LIBS = 
CFLAGS =  -Wall -pedantic -std=c11 -ggdb -I../lib -I../common

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# dependences
crawler.o: ../libcs50/bag.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/memory.h ../libcs50/set.h ../libcs50/file.h ../libcs50/jhash.h ../common/pagedir.h

test: $(PROG) testing.sh
	mkdir dir
	bash -v ./testing.sh
	rm -rf dir

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
