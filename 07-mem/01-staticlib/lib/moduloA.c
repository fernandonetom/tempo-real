/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição:
 * Arquivo de implementaçao do moduloA da biblioteca 'libdee'
 *
 * Compilação: 
 * gcc -std=c99 -o moduloA.o -c moduloA.c
 * ar rcu libdee.a moduloA.o
 * 
 * Forma geral para criação de bibliotecas:
 * ar rcu libnome.a moduloA.o moduloB.o fft.o calculo.o 
 *
 */

#include "libdee.h" 


int funcA(int a, int b) {
	return (a+b)-(a*b);
}
