#Esse Makefile gera executaveis de todos os arquivos .c

SRCS    = $(wildcard *.c) $(wildcard *.cpp)
PROGS   = $(patsubst %.c,%,$(patsubst %.cpp,%,$(SRCS)))
CFLAGS  = -std=c99 -fopenmp
LDFLAGS = -lpthread -lm

all: $(PROGS)

%: %.c
	gcc $(CFLAGS)  -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROGS)
