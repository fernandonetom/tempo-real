#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int create_socket(int server, const char *addr) {
	// Create a UNIX-domain socket.
	printf("Creating socket...\n");
	int s = socket(AF_UNIX, SOCK_STREAM, 0);
	
	// socket(...) returns -1 on error.
	if (s == -1) {
		printf("socket(...) failed!\n");
		return -1;
	}
	
	// Unix sockets presents themselves like special files, much like FIFOs.
	// The sockaddr_un structure is used to describe a socket configuration.
	// All this socket needs is a file name.
	struct sockaddr_un sa;
	sa.sun_family = AF_UNIX; // Mandatory
	strcpy(sa.sun_path, addr);
	
	// Server and client socket creation is slightly different...
	int r;
	if (server) {
		// We bind(...) a server to an address, and
		// later we will accept(...) connections from it.
		r = bind(s, (struct sockaddr *)&sa, sizeof(sa));
		
		// Bind may fail and return -1. If it does not fail,
		if (r != -1) {
			// then we tell the socket to start listening for clients.
			r = listen(s, 10);
		}
	} else {
		// We connect(...) a client to an address.
		r = connect(s, (struct sockaddr *)&sa, sizeof(sa));
	}
	
	// bind, listen and connect all return -1 on error.
	if (r == -1) {
		// A failed socket is useless, close it.
		close(s);
		
		printf("bind(...) or connect(...) failed!\n");
		return -1;
	}
	
	// Server socket is now fully configured.
	printf("Socket ready.\n");
	return s;
}
