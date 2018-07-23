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
	printf("fork returned %d\n", i);
	return 0;
}
