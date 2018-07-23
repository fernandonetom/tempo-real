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
 * OBS: Mude o valor N_THREADS para 2,4, 8 e observe o tempo cosumido para o processamento
 */

#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


//define uma macro com o número de threads a serem criadas
#define N_THREADS    2
#define N_ELEMENTOS  2048*10000
#define N_POR_TH     (N_ELEMENTOS/N_THREADS)

double darray[N_ELEMENTOS]; //um array de interos com N_ELEMENTOS


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


  /* inicializa a semente randomica */
  srand (time(NULL));

  //preenche o array com valores aleatórios
  for (int i=0;i<N_ELEMENTOS; i++)
    darray[i] = i*rand();

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

  printf ("thread %d, faixa :%d->%d\n" , meu_id , meu_id*N_POR_TH, (meu_id + 1)*N_POR_TH );


  //cada thread modifica uma região independente do array
  //dessa forma, todas podem trabalhar em paralelo  
  for (int i = meu_id*N_POR_TH ; i < (meu_id + 1)*N_POR_TH  ; i++) {

    //faz umas continhas inúteis pra esquentar a CPU
    darray[i] *= 0.000245 ; 
    for (double j=0; j < 3.2 ; j += 0.003) darray[i] += j;
                           
  }

  printf ("thread %d, terminou de calcular\n", meu_id);

  return 0;
}
