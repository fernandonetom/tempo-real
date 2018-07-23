/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição:
 * Criação e uso de biblioteca estática
 *
 * Compilação: 
 * gcc -std=c99 -I../lib -o main main.c -L../ -ldee
 *
 */


#include <libdee.h> //inclui a lib
#include <stdio.h>


int main(void) {
	
	funcA(10,20);
	
	printf("%d\n" , funcA(5,8));
	
	return 0;
}
