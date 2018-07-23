#Esse Makefile gera executaveis de todos os arquivos .c

SRCS    = $(wildcard *.c)
PROGS   = $(patsubst %.c,%,$(SRCS))
CFLAGS  = -std=gnu11
LDFLAGS = -lpthread -lm
#LDFLAGS = -lpthread -lSDL

all: $(PROGS)

%: %.c
	gcc $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROGS)
