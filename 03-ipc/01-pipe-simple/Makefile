#Esse Makefile gera executaveis de todos os arquivos .c

SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,%,$(SRCS))
CFLAGS = -std=c99

all: $(PROGS)

%: %.c
	gcc $(CFLAGS)  -o $@ $<

clean:
	rm -f $(PROGS)
