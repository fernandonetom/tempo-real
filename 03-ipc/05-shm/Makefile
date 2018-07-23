all: reader worker writer

CFLAGS += -std=c99

reader: reader.o shared_memory.o shared_memory.h
	gcc -o reader reader.o shared_memory.o -lrt -lpthread

worker: worker.o shared_memory.o shared_memory.h
	gcc -o worker worker.o shared_memory.o -lrt -lpthread

writer: writer.o shared_memory.o shared_memory.h
	gcc -o writer writer.o shared_memory.o -lrt -lpthread

clean:
	rm -f reader worker writer
