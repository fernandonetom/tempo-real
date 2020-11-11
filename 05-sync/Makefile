#Esse Makefile gera executaveis de todos os arquivos .c

SRCS    = $(wildcard *.cpp) 
PROGS   = $(patsubst %.cpp,%,$(SRCS))
LDFLAGS = -lpthread -lm
#LDFLAGS = -lpthread -lSDL

all: $(PROGS)

%: %.cpp
	g++ $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROGS)
