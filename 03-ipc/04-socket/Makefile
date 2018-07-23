BIN=unix-client unix-server inet-client inet-server

CFLAGS += -std=c99

all: $(BIN)

unix-client: client.o create_socket_unix.o
	gcc -o $@ client.o create_socket_unix.o

unix-server: server.o create_socket_unix.o
	gcc -o $@ server.o create_socket_unix.o

inet-client: client.o create_socket_inet.o
	gcc -o $@ client.o create_socket_inet.o

inet-server: server.o create_socket_inet.o
	gcc -o $@ server.o create_socket_inet.o

clean:
	rm -f *.o $(BIN)
