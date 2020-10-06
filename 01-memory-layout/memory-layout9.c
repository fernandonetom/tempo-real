/**
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Lucas V. Hartmann
 *
 * Descrição:
 * Layout de memória de um processo
 * 
 * Compilação: 
 * gcc -g -O0 -std=c99 -o prog prog.c
 * OBS: substituir 'prog' pelo nome do arquivo
 *
 *  
 * Analisar a saída dos seguintes comandos:
 * 
 * size prog
 * objdump -S -j .data  prog > prog.asm ; less -S prog.asm 
 * objdump -S -j .text  prog > prog.asm ; less -S prog.asm 
 * objdump -S -j .bss   prog > prog.asm ; less -S prog.asm 
 *
 * Execute o programa e analise o resultado, considere como funciona a pilha.
 */
#include <stdio.h>

unsigned long factorial(unsigned long n) {
	int f = 1;
	printf("F: n[%8d],  f[%8d]  =>  &f[0x%p]\n", n, f, &f);
	if (n>1) f = n*factorial(n-1);
	printf("F: n[%8d],  f[%8d]  =>  &f[0x%p]\n", n, f, &f);
	return f;
}

unsigned long combinations(unsigned long select, unsigned long from) {
	printf("C: &select[0x%p] &from[0x%p]\n", &select, &from);
	return factorial(from) / factorial(select) / factorial(from-select);
}

int main(void){
	int a=4, b=12;
	int c = combinations(a, b);
	printf("When picking %d elements from a set of %d, you may end up with %d unique combinations.\n", a,b,c);
	return 0;
}
