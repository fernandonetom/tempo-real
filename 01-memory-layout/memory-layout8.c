/**
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
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

  printf("text\t%p\t%p\n", &main, &printf);
  printf("bss \t%p\t%p\n", &uninit1, &uninit2);
  printf("data\t%p\t%p\n", &init1, &init2);
  printf("auto\t%p\t%p\n", &local1, &local2);
  
  return 0;
}
