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
int g = 34;

void func() {
    static int i = 25;
    static int j = 12;
    int local = 3;
}

int main(void){
   int i;
   static int j = 4;
   return 0;
}
