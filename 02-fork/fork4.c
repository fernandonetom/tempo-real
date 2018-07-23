/**
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * (c) 2015 - Isaac Maia Pessoa
 * (c) 2016 - Lucas Vinicius Hartmann
 *
 * Descrição:
 * Criação de dois processos compartilhando descritor de arquivos
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
	pid_t pid;
	FILE  *arq; //decara um descritor de arquivo

	arq = fopen("fork4.txt", "w"); //abrir o arquivo para r/w
	if (arq == NULL) {
		perror("erro ao abrir o arquivo");
	}

	pid = fork(); //cria um novo processo

	if (pid == 0) { /* processo filho */
		fprintf(arq, "FILHO:%d\n",getpid()); //escreve no arquivo
	} else if (pid > 0) { /* processo pai */
		wait(NULL); //espera que o filho termine antes de escrever no arquivo
		fprintf(arq, "PAI:%d\n",getpid()); //escreve no arquivo

		fclose(arq); //fecha o descritor
	}

	printf("Ver arquivo 'fork4.txt'\n");
	return 0;
}
