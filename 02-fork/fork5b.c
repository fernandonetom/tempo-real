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

int main(int argc, char *argv[]) {
	int i;
	for (i=0; i<argc; ++i) {
		printf("argv[%d] = \"%s\"\n", i, argv[i]);
	}
	return 0;
}
