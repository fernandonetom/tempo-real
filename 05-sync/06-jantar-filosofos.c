/*
  Jantar dos Filósofos
  Cinco filósifos estão ao redor de uma mesa redonda com um prato de spagetti ao centro
  Ao lado de cada prato existe um garfo, totalizando cinco garfos
  Cada filósifo pode estar em um dos seguintes estados: comendo ou pensando
  Quando um filósifo fica com fome, ele deve pegar os dois garfos, um de cada vez,
  o da sua direita e o da sua esquerda para poder começar a comer. a ordem não importa.
  Ele então começa a comer e depois de algum intervalo de tempo fixo deve colocar os garfos
  de volta na mesa para que os outros possam comer.   

  O objetivo é fazer a sincronização entre os filósifos para que todos possam comer, sem
  que nenhum morra de fome e sem que exista deadlocks.

  
  Cada filósofos pode realizar as seguintes operações:
   - pegar um garfo
   - devolver um garfo
   - comer
   - pensar
  
 */
 
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
 
#define N 5 
#define PENSANDO 0
#define FAMINTO  1
#define COMENDO  2

#define ESQUERDA (ph_num+4)%N
#define DIREITA (ph_num+1)%N
 
sem_t mutex; 
sem_t semaf[N]; 
 
void * filosofo(void *num);
void   pega_garfo(int);
void   libera_garfo(int);
void   testa(int);
 
int estado[N];
int num_filos[N]={0,1,2,3,4};
 
int main()
{
    int i;
    pthread_t thread_id[N];
    sem_init(&mutex,0,1);
    for(i=0;i<N;i++)
        sem_init(&semaf[i],0,0);
    for(i=0;i<N;i++)
    {
        pthread_create(&thread_id[i],NULL,filosofo,&num_filos[i]);
        printf("Filosofo %d está pensando\n",i+1);
    }
    for(i=0;i<N;i++)
        pthread_join(thread_id[i],NULL);
}
 
void *filosofo(void *num)
{
    while(1)
    {
        int *i = num;
        sleep(1);
        pega_garfo(*i);
        sleep(0);
        libera_garfo(*i);
    }
}
 
void pega_garfo(int ph_num)
{
    sem_wait(&mutex);
    estado[ph_num] = FAMINTO;
    printf("Filosofo %d está comendo\n",ph_num+1);
    testa(ph_num);
    sem_post(&mutex);
    sem_wait(&semaf[ph_num]);
    sleep(1);
}
 
void testa(int ph_num)
{
    if (estado[ph_num] == FAMINTO && estado[ESQUERDA] != COMENDO && estado[DIREITA] != COMENDO)
    {
        estado[ph_num] = COMENDO;
        sleep(2);
        printf("Filosofo %d pega garfo %d e %d\n",ph_num+1,ESQUERDA+1,ph_num+1);
        printf("Filosofo %d está comendo\n",ph_num+1);
        sem_post(&semaf[ph_num]);
    }
}
 
void libera_garfo(int ph_num)
{
    sem_wait(&mutex);
    estado[ph_num] = PENSANDO;
    printf("Filosofo %d está liberando garfo %d e %d\n",ph_num+1,ESQUERDA+1,ph_num+1);
    printf("Filosofo %d está pensando\n",ph_num+1);
    testa(ESQUERDA);
    testa(DIREITA);
    sem_post(&mutex);
}
