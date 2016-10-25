SHELL=/bin/bash
CC=gcc
CFLAGS=-Wall -g
LDLIBS=-lpq
PG_CONFIG=pg_config
CFLAGS+=-I $(shell $(PG_CONFIG) --includedir)

.PHONY: all clean

all: cpgsql

cpgsql: cpgsql.o

clean:
	$(RM) *.o cpgsql
