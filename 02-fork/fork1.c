/**
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * (c) 2015 - Isaac Maia Pessoa
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
	printf("A:%d\n",getpid());
	
	fork();
	
	printf("B:%d\n",getpid());
	
	fork();
	
	printf("C:%d\n",getpid());
	
	fork();
	
	printf("D:%d\n",getpid());
}
