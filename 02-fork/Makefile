#Esse Makefile gera executaveis de todos os arquivos .c

SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,%,$(SRCS))
OBJS = $(SRCS:.c=.o)
TEMPS = $(SRCS:.c=.txt)

CFLAGS = -std=c99 -g

all: $(PROGS)

%: %.c
	gcc $(CFLAGS)  -o $@ $<

clean:
	@rm -f $(PROGS)	$(OBJS) $(TEMPS)
	@echo "Limpo!"
