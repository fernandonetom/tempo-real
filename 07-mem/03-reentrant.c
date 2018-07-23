/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição:
 * Memória: Segmentação
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c -lpthread
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */

#include <stdio.h>
#include <stdlib.h>


void func(void);

int main(int argc, char *argv[]) {    
	
    int * ptr = malloc(sizeof(int));
	    
	    
	
//       heap
//       |      |
//       |      |
//       |      |
// ptr ->|__45__|

	
    printf("endereço base do heap:%p\n",  ptr); 
	
    func();	
    return 0;
}



void func(void){	
     int * x = malloc(sizeof(int));
//       heap
//       |      |
//       |      |
//  x -> |      |
//       |__45__|
	
	
     //x -= 2*sizeof(int);
	     
     printf("x:%p\n",x);
     x = malloc(sizeof(int));
     printf("x:%p\n",x);
	
}