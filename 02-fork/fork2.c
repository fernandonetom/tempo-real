/**
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * (c) 2015 - Isaac Maia Pessoa
 * (c) 2016 - Lucas Vinicius Hartmann
 *
 * Descrição:
 * Criação de processos e escalonamento
 *
 * Compilação:
 * gcc -std=c99 -o prog prog.c
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */

#include <stdio.h>
#include <sys/types.h>  //pid_t
#include <sys/wait.h>   //wait
#include <unistd.h>

int main(int argc, char * argv) {
	pid_t pid;
	pid = fork(); //cria um novo processo

	if (pid == 0) { /* processo filho */
		printf ("FILHO: pid=%d, getpid()=%d, getppid()=%d\n",
			pid, getpid(), getppid()
		);
		return 0;
	}

	else if (pid > 0) { /* processo pai */
		printf ("PAI: pid=%d, getpid()=%d, getppid()=%d\n",
			pid, getpid(), getppid()
		);
		
		wait(0); // Espera um sinal, ver SIGCHLD em signal.h
		return 0;
	}
}
