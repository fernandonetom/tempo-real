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

int global;
static int  i = 5;

int main(void){ 
   int arr[5] = {5,4,3,2,1};
   return 0;
}
