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
	
	if (i == 0) {
		char *param[] = {
			"hello",
			"there",
			"you!",
			0
		};
		execv("./fork5b", param);
	} else {
		wait(0);
		printf("Child is done.\n");
	}
	
	return 0;
}
