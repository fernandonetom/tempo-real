/**
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 * Lucas Vinicius Hartmann
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
 */

#include <stdio.h>

int uninit1;   //variável global não inicializada
int uninit2;   //variável global não inicializada

int init1 = 1; //variável global inicializada
int init2 = 2; //variável global inicializada

int main (void)
{
  char local1 = 1; //variável local inicializada
  char local2 = 2; //variável local inicializada

  printf("text...:    &main[%016p]   &printf[%016p]\n", &main,    &printf );
  printf("bss....: &uninit1[%016p]  &uninit2[%016p]\n", &uninit1, &uninit2);
  printf("data...:   &init1[%016p]    &init2[%016p]\n", &init1,   &init2  );
  printf("stack..:  &local1[%016p]   &local2[%016p]\n", &local1,  &local2 );
  
  return 0;
}
