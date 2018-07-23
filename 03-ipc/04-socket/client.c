/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * (c) 2015 Isaac Maia Pessoa
 * (c) 2016 Lucas V. Hartmann <lhartmann@github.com>
 *
 * Descrição:
 * Comunicação entre processos usando Named Pipes(FIFOs)
 * Processo Escritor
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "create_socket.h"

#define TAM_MSG 1024

int main(int argc, char *argv[]) {
	// Require an address to be supplied from the command line
	if (argc != 2) {
		printf("Please supply and address to connect to.\n");
		return 1;
	}
	
	// Creates a client socket and terminate on error
    int sock = create_socket(0, argv[1]);
	if (sock == -1) return 1;

	// Enter the message loop
	while (1) {
		// Ask the user for a message
		printf("Type a message or END: ");
		char msg[TAM_MSG];
		scanf("%s",msg);
		
		// If the user enters END then break away from the loop.
		if (strcmp(msg, "END") == 0) break;
		
		// Find out the message size.
		ssize_t len  = strlen(msg);
		
		// Send the message, null terminator not included.
		// "sent" gets the number of bytes actually written.
		ssize_t sent = write(sock,  msg , len);
		
		// Make sure the message went whole.
		if (sent != len) {
			printf("Failed writing.\n");
			return 1;
		}
		
		// Read response from server
		ssize_t sz = read(sock, msg, TAM_MSG);
		
		// read returns -1 if read has failed.
		if (sz == -1) break;
		
		// If read size is zero then something went wrong too.
		if (!sz) break;
		
		// Add the null terminator to make sure it is a valid C string
		msg[sz] = 0;
		
		// Print back the received message.
		printf("Got: \"%s\"\n", msg);
	}

	// Close the FIFO and remove it.
	close(sock);

	// All went well, let the OS know it.
	return 0;
}
