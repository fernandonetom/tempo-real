/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição:
 * Política de escalonamento tempo real
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c -lpthread
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <sched.h>    //sched_{setaffinity , setscheduler, ...}
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define N_THREADS 2
#define NCPUS      sysconf(_SC_NPROCESSORS_ONLN);

typedef struct
{
   int       id;  //o meu numero
   int       cpu; //a minha cpu
}thread_params_t;



void *th_func(void * param); 
void check_realtime();

 
int main(void)  
{
  //um array de threads
  pthread_t        pthread[N_THREADS];
  thread_params_t  param_array[N_THREADS];


  //define uma estrutura para manusear os parametros de escalonamento do processo
  struct sched_param sched_opts;
  sched_opts.sched_priority = sched_get_priority_max(SCHED_RR);

  /* Escolher uma das tres - SCHED_FIFO, SCHED_RR ou  SCHED_OTHER */
  if ( sched_setscheduler(0, SCHED_RR,  &sched_opts) ) {
            fprintf(stderr, "%s%s\n",
                 "Incapaz de atribuir a nova politica de escalonamento:",
                 strerror(errno));
  }


  /** 
   * Para ptherads indivuais, ver o manual das seguintes funcoes: 
   * pthread_attr_setschedparam   
   * pthread_attr_setinheritsched 
   * pthread_attr_setschedpolicy
   */

  for (int i = 0; i < N_THREADS; i++) {
    param_array[i].id = i; //o valor de i será o identificador de cada thread
    param_array[i].cpu = (i+1) % NCPUS; //a cpu onde a thread irá rodar

    //cria a pthread usando a politica definida acima em attr
    pthread_create (&pthread[i], NULL , th_func, &param_array[i]);
  }


  for (int i = 0; i < N_THREADS; i++)
    pthread_join (pthread[i], NULL);


  return 0;
}


void *th_func(void * param)
{

  thread_params_t * p = (thread_params_t*)param;
  cpu_set_t cpuset;  

  //man CPU_SET 
  CPU_ZERO(&cpuset);         //zera a mascara
  CPU_SET(p->cpu ,&cpuset);  //seta o bit correspondente a CPU na mascara

  //0 = thread que chamou sched_setaffinity
  int result = sched_setaffinity(0, sizeof(cpuset), &cpuset);

  if (result) {
    fprintf(stderr, "%s%d%s%s\n",
            "erro em cpu_affinity da thread [", p->id, "]: ",
            strerror(errno));
  }

  printf("Thread %d na cpu %d\n", p->id, p->cpu);

  for ( ; ; )
        ; //torra a CPU

  return 0;
}
