#Esse Makefile gera executaveis de todos os arquivos .c

SRCS    = $(wildcard *.c)
PROGS   = $(patsubst %.c,%,$(SRCS))
CFLAGS  = -std=gnu11 -D_GNU_SOURCE -Wall 
LDFLAGS = -lrt

all: $(PROGS)

%: %.c
	gcc $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROGS)
