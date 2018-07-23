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

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include "shared_memory.h"

int main() {
	// Open an map shared memory, and make sure it worked.
	int smfd;
	struct shared_memory *smp;
	if (shared_memory_open(&smfd, &smp,0 ) == -1) return 1;
	
	// Enter the message loop
	while (1) {
		// Ask the user for a message
		printf("Type a message or END: ");
		char msg[1024];
		scanf("%s",msg);
		
		// Find out the message size.
		ssize_t len  = strlen(msg);
		
		// Wait for buffer to be free before sending
		sem_wait(&smp->contains_nothing);
		
		// Lock the mutex, copy the message, unlock the mutex.
		sem_wait(&smp->mutex);
		strcpy(smp->data, msg);
		sem_post(&smp->mutex);
		
		// Signal that the buffer now contains lowercase text
		sem_post(&smp->contains_lowercase);
		
		// If the user entered END then break away from the loop.
		if (strcmp(msg, "END") == 0) break;
	}

	// Close the FIFO and remove it.
	shared_memory_close(&smfd, &smp, 0);

	// All went well, let the OS know it.
	return 0;
}
