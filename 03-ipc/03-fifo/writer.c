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

#define TAM_MSG 1024

int main(int argc, char *argv[]) {
	int   fifo_fd;
	char *nome_fifo = "/tmp/fifo-nb-str";
	char  msg[TAM_MSG] = "";

	// Creates a FIFO / Named Pipe
	mkfifo(nome_fifo, 0666);

	// Open the FIFO for writing.
	// This blocks until reader opens it too.
	printf("Waiting for a FIFO reader...\n");
	fifo_fd = open(nome_fifo, O_WRONLY);
	
	// open returns -1 on error, so let's make sure it didn't.
	if (fifo_fd == -1) {
		printf("Failed!\n");
		return 1;
	}
	
	// fifo_fd is now a FIFO handle, and a client is connected.
	printf("Connected!\n");

	// Enter the message loop
	while (1) {
		// Ask the user for a message
		printf("Type a message or END: ");
		scanf("%s",msg);
		
		// If the user enters END then break away from the loop.
		if (strcmp(msg, "END") == 0) break;
		
		// Find out the message size.
		int len  = strlen(msg);
		
		// Send the message, null terminator not included.
		// "sent" gets the number of bytes actually written.
		int sent = write(fifo_fd,  msg , len);
		
		// Make sure the message went whole.
		if (sent != len) {
			printf("Failed writing.\n");
			return 1;
		}
	}

	// Close the FIFO and remove it.
	close(fifo_fd);
	unlink(nome_fifo);

	// All went well, let the OS know it.
	return 0;
}
