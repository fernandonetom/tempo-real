/**
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição:
 * Comunicação entre processos usando Named Pipes(FIFOs)
 * Processo Leitor
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include "create_socket.h"

#define TAM_MSG 1024

int main(int argc, char *argv[]) {
	// Require an address to be supplied from the command line
	if (argc != 2) {
		printf("Please supply and address to connect to.\n");
		return 1;
	}
	
	// Creates a server socket and terminate on error
    int server = create_socket(1, argv[1]);
	if (server == -1) return 1;
    
	// Listener loop
	while (1) {
		printf("Ready for clients...\n");
		
		// Wait for connections on the socket server.
		// Each client gets a new private socket.
		int client = accept(server, 0, 0);
		printf("Got a client!\n");
		
		// Client message loop:
		while (1) {
			// Reads a message.
			char  msg[TAM_MSG ];
			ssize_t sz = read(client, msg, TAM_MSG);
			
			// read returns -1 if read has failed.
			if (sz == -1) break;
			
			// If read size is zero then something went wrong too.
			if (!sz) break;
			
			// Add the null terminator to make sure it is a valid C string
			msg[sz] = 0;
			
			// Print back the received message.
			printf("Got: \"%s\"\n", msg);
			
			// Change the message to uppercase
			for (char *p=msg; *p; p++) *p = toupper(*p);
			
			// Send back to the client
			write(client, msg, sz);
		}
		
		// Terminate the client connection.
		printf("Client is disconnected.\n");
		close(client);
	}

	// Close and remove the server socket
	close(server);
	unlink(argv[1]);

	// All went well, let the OS know it.
	return 0;
}

