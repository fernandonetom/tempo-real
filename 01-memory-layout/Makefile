#Esse Makefile gera executaveis de todos os arquivos .c

SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,%,$(SRCS))
CFLAGS = -std=c99 -g -O0

all: $(PROGS)

%: %.c
	gcc $(CFLAGS)  -o $@ $<

clean:
	$(RM) $(PROGS) *.asm *.o
