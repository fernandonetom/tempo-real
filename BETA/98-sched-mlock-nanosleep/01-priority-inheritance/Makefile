#Esse Makefile gera executaveis de todos os arquivos .c

SRCS    = $(wildcard *.c)
PROGS   = $(patsubst %.c,%,$(SRCS))
CFLAGS  = -std=gnu11 -O0
LDFLAGS = -lpthread -lm -lrt
#LDFLAGS = -lpthread -lSDL

all: $(PROGS)

%: %.c
	gcc $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROGS)

perf:
	sudo perf timechart record ./02-with-mutex
	sudo perf timechart -w 100000
	xdg-open output.svg

