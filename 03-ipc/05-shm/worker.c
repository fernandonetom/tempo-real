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
#include <ctype.h>
#include <string.h>
#include "shared_memory.h"

int main() {
	// Open, map and init shared memory contents, then make sure it worked.
	int smfd;
	struct shared_memory *smp;
	if (shared_memory_open(&smfd, &smp, 1) == -1) return 1;
	
	// Enter the message loop
	int keep_looping = 1;
	while (keep_looping) {
		// Wait for buffer to contain a lowercase message
		sem_wait(&smp->contains_lowercase);
		
		// Lock the mutex
		sem_wait(&smp->mutex);
		
		// Change text to uppercase
		for (char *p=smp->data; *p; p++) *p = toupper(*p);
		
		// If the text is "END" then stop looping
		if (strcmp(smp->data, "END") == 0) keep_looping = 0;
		
		// Release the mutex
		sem_post(&smp->mutex);
		
		// Signal that the buffer now contains uppercase text
		sem_post(&smp->contains_uppercase);
	}

	// Release the shared memory
	shared_memory_close(&smfd, &smp, 1);

	// All went well, let the OS know it.
	return 0;
}
