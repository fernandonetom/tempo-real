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


int main(int argc, char *argv[]) {    
    printf("segmento de código : %p\n", &main);    
    printf("heap...............: %p\n", (void *)malloc(0));
    int x = 3;
    printf("pilha(stack).......: %p\n", (void *) &x);
    return x;
}
