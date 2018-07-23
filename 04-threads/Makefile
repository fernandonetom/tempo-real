#Esse Makefile gera executaveis de todos os arquivos .c

SRCS    = $(wildcard *.c)
PROGS   = $(patsubst %.c,%,$(SRCS))
CFLAGS  = -std=c99 -fopenmp
LDFLAGS = -lpthread -lm

all: $(PROGS)

%: %.c
	gcc $(CFLAGS)  -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROGS)
