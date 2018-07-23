CFLAGS  += -std=c99
LDFLAGS += -lrt -lpthread

all: reader worker writer

%: %.c
	gcc -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f reader worker writer
	rm -f *.o
