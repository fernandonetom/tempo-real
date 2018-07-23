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
	int keep_looping = 1;
	while (keep_looping) {
		// Wait for buffer to contain uppercase text
		sem_wait(&smp->contains_uppercase);
		
		// Lock the mutex
		sem_wait(&smp->mutex);
		
		// Print the message
		printf("GOT: \"%s\"\n", smp->data);
		
		// If the message is "END" then stop looping
		if (strcmp(smp->data, "END") == 0) keep_looping = 0;
		
		// Release the mutex
		sem_post(&smp->mutex);
		
		// Signal that the buffer now contains nothing (useful)
		sem_post(&smp->contains_nothing);
	}

	// Close the shared memory region.
	shared_memory_close(&smfd, &smp, 0);

	// All went well, let the OS know it.
	return 0;
}
