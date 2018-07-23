/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição:
 * Memória: Paginação
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c -lpthread
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //getpid
#include <sys/mman.h> //mlockall

#define MB (1 << 20)

int main(int argc, char *argv[]) {    
    
    void * ptr = (void *)malloc(10*MB); //Aloca 10MB de memória no heap
		
    printf("PID:%d\n",(int)getpid());
	
    printf("heap:%p\n",ptr);
    
    printf("veja as colunas VIRT,RES e SHR no 'top' e depois pressione alguma tecla...\n");
	
    getchar(); 
		
    //tenta manter residente as paginas atuais e as futuras
    if ( mlockall(MCL_CURRENT | MCL_FUTURE) != 0 ){
	    perror("Erro: você não tem privilégios para usar mlockall!\n");
    }
	
    printf("mlockall... : agora veja novamente as colunas VIRT,RES e SHR no 'top'\n");
	
    getchar();
	
    return 0;
}
