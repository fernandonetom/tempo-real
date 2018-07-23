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
#include <errno.h>

#define TAM_MSG 1024

int main() {
    int   fifo_fd;
    char *nome_fifo = "/tmp/fifo-nb-str";
    char  msg[TAM_MSG ];

    // Open the FIFO for reading.
	// Must be executed AFTER the writer creates it.
	printf("Connecting to the FIFO...\n");
    fifo_fd = open(nome_fifo, O_RDONLY);
	
	// open returns -1 on error, so let's make sure it didn't.
	if (fifo_fd == -1) {
		printf("Failed!\n");
		return 1;
	}
	
	// Message receiver loop
	printf("Ready for messages...\n");
	while (1) {
		// Reads a message.
		ssize_t sz = read(fifo_fd, msg, TAM_MSG);
		
		// read returns -1 if read has failed.
		if (sz == -1) {
			// Anything else is an error.
			printf("Failed!\n");
			return 1;
		}
		
		// If read size is zero then the FIFO has no writers.
		if (!sz) break;
		
		// Add the null terminator to make sure it is a valid C string
		msg[sz] = 0;
		
		// Print back the received message.
		printf("Got: \"%s\"\n", msg);
	}

	// Close the FIFO
	close(fifo_fd);

	// All went well, let the OS know it.
	return 0;
}

