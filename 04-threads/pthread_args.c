/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição:
 * Threads: Passagem de parametros para as threads
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


//define uma macro com o número de threads a serem criadas
#define N_THREADS 6


//esse sempre será o protótipo de uma função para pthread_create
void *th_func (void *param);


//essa estrutura serve para associarmos cada thread a um identificador único
typedef struct
{
   pthread_t pth; //a thread
   int       id;  //o identificador da thread
}thread_t;



int
main (void)
{
  //um array de threads
  thread_t thread_array[N_THREADS];


  //pthread_create recebe como parametro a função que deve ser executada pela thread
  //O ultimo parametro passa um apontador para a estrutura 'thread_t' que carrega o 'id' da thread
  for (int i = 0; i < N_THREADS; i++) {
    thread_array[i].id = i; //o valor de i será o identificador de cada thread
    pthread_create (&thread_array[i].pth, NULL, th_func, &thread_array[i]);
  }


  //a thread 'main' deve esperar que todas as threads terminem antes de continuar. 
  //Caso contrário o main terminaria o processo antes das threads terminarem
  //Usa-se 'ptrhead_join' para fazer o main esperar por uma thread antes de continuar
  for (int i = 0; i < N_THREADS; i++)
    pthread_join (thread_array[i].pth, NULL);

  return 0;
}



//cada thread possui uma pilha privada com a cópia da função
void *
th_func (void *param)
{
  
  thread_t *th = (thread_t*)param;
  int meu_id = th->id;

  printf ("Eu sou a thread %d\n", meu_id);
  return 0;
}
