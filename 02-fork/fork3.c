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
#include <sys/types.h>
#include <unistd.h>

int global_var = 5;

int main() {
	pid_t pid;
	int local_var = 0;
	
	pid = fork();
	
	if (pid == 0) { /* processo filho */
		global_var += 15;
		
		printf ("FILHO..: getpid()=%d, getppid()=%d, global_var=%d, local_var=%d\n",
			getpid(), getppid(), global_var, local_var
		);
		sleep(4); //espera 4s
		printf ("FILHO..: getpid()=%d, getppid()=%d, global_var=%d, local_var=%d\n",
			getpid(), getppid(), global_var, local_var
		); 
		return 0;
	} else if (pid > 0) { /* processo pai */
		local_var++;

		printf ("PAI....: getpid()=%d, getppid()=%d, global_var=%d, local_var=%d\n",
			getpid(), getppid(), global_var, local_var
		); 
		
		return 0;
	}
}
