/**
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
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

void child(int wrpipe) {
	for (int i=0; i<10; ++i) {
		// Create a message string
		char msg[TAM_MSG];
		sprintf(msg, "Message %d.", i);
		
		// Just so we can see something...
		printf("C: Sending \"%s\"...\n", msg);
		
		// Send the message (not the null terminator)
		write(wrpipe, msg, strlen(msg));
	}
}

void parent(int rdpipe) {
	while(1) {
		// Reads a message
		printf("P: Waiting for messages...\n");
		char msg[TAM_MSG];
		int n = read(rdpipe, msg, TAM_MSG);
		
		// read returns -1 on error
		if (n == -1) {
			printf("P: read error.\n");
			return;
		}
		
		// read return 0 if no more data is available
		if (n == 0) break;
		
		// Otherwise read returns the number of bytes read.
		// Use it to place the null-terminator.
		msg[n] = 0;
		
		// Print the message
		printf("P: Got \"%s\"\n", msg);
	}
}

int main(int argc, char *argv[]) {
	// Create a pipe.
	int fdp[2];
	int r = pipe(fdp);
	
	// pipe returns -1 on error.
	if (r == -1) {
		printf("Failed to create pipe.\n");
		return 1;
	}
	
	// fdp now holds file descriptors for both ends of the pipe.
	// fdp[0] is the reading end
	// fdp[1] is the writing end
	
	// Let's create a child process to do the writing:
	pid_t pid = fork();
	if (pid) {
		// Parent code needs only the reading end of the pipe
		close(fdp[1]); // Close the write end
		parent(fdp[0]);
		
		// Close the other end when done.
		close(fdp[0]);
	} else {
		// Child code needs only the writing end of the pipe
		close(fdp[0]); // Close the read end.
		child(fdp[1]);
		
		// Close the other end once done.
		close(fdp[1]);
	}
	
	return 0;
}
