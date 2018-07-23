LDFLAGS+=-lrt

all: reader writer

clean:
	rm -f reader writer

reader: reader.cpp
	g++ -o $@ $< $(CFLAGS) $(LDFLAGS)

writer: writer.cpp
	g++ -o $@ $< $(CFLAGS) $(LDFLAGS)

