/**
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * (c) 2016 - Lucas Vinicius Hartmann
 *
 * Descrição:
 * Criação de multiplos processos
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int i = fork();
	
	if (i == 0) { // Child process
		printf("Child is sleeping...\n");
		if (1) { // Change between 0 to 1, and repeat test.
			sleep(60);
		} else {
			execl("/bin/sleep","sleep","60",0);
		}
		printf("Child is done sleeping.\n");
	} else { // Parent process
		printf("Parent is waiting for child...\n");
		wait(0);
		printf("Child is dead. :-(\n");
	}
	
	return 0;
}
