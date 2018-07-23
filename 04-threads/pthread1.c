/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição:
 * Threads: Criação de threads
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c -lpthread
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */

#include <sys/time.h>
#include <stdio.h>
#include <pthread.h> 
#include <errno.h>


//Essa será a função executada pela thread
//Esse sempre será o protótipo de uma função para pthread_create
void *th_func(void * param); 

 
int main(void)  
{
  pthread_t thread1; //deaclara uma thread
  pthread_t thread2; //thread 2
  
  pthread_create(&thread1,NULL,th_func,NULL); //cria a thread 1 passando a função como argumento
  pthread_create(&thread2,NULL,th_func,NULL); //cria a thread 2 passando a mesma função como argumento


  //a thread 'main' espera que thread1 termine, antes de continuar
  //caso contrário o main poderia chegar ao fim e terminar o processo 
  //antes que a thread acabesse de executar até o fim
  pthread_join(thread1,NULL); 
  pthread_join(thread2,NULL); 

  return 0;
}


//implementação da função
//cada thread possui uma pilha privada com a cópia da função
void *th_func(void * param)
{
  for (int i=0 ; i < 1000 ; i++)
    printf("Thread: %d\n", i);
  return 0;
}

