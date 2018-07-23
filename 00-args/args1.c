/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição:
 * Criação de processos
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char ** argv)
{
	for (int i=0; i < argc ; i++){
		printf("%s\n",argv[i]);
	}
}
