/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição: 
 * Semáforos: Produtor e Consumudor
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c -lpthread
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */


#include <stdlib.h>    //rand
#include <stdio.h>      
#include <pthread.h>   //pthrea_t
#include <semaphore.h> //sem_t
#include <string.h>    //memset
#include <unistd.h>    //sleep



//funçao do produdor
void *fn_prod(void * arg); 

//funcao do consumidor
void *fn_cons(void * arg); 


#define TAM_BUFFER  20
int     buffer[TAM_BUFFER];

sem_t ocup;  //semaforo para inidicar a ocupação/preenchimento do buffer
sem_t disp;  //semaforo para indicar a disponibilidade de vagas no buffer
sem_t mutex; //semaforo para a região crítica/ podemos usar um mutex_t no lugar de sem_t
int   fim;   //sinaliza o fim da execução

int main(void)  
{
  pthread_t th_prod;
  pthread_t th_cons;
  
  //preenche o buffer com zeros
  memset(buffer, 0 , TAM_BUFFER*sizeof(int));

  //inicia os semáforos, o segundo argumento (0) indica se o semáforo será compartilhado entre processos ou threads
  sem_init(&ocup,0,0);            //inicialmente o buffer está vazio, iniciamos ocup com 0 pois não existe ocupação no mesmo
  sem_init(&disp,0,TAM_BUFFER);   //O número de vagas disponíveis no buffer
  sem_init(&mutex,0,1);           //Isso é um semáforo binário para proteger a região crítica

  //inicializa a semente randômica
  srand (time(NULL) );
  
  fim = 0;
  
  pthread_create(&th_prod,NULL,fn_prod,NULL);
  pthread_create(&th_cons,NULL,fn_cons,NULL); 

  pthread_join(th_prod,NULL); 
  pthread_join(th_cons,NULL); 
  return 0;
}


void *fn_prod(void * arg){
    int cont_p = 0; //o contador interno do produtor
    while (!fim){
      sem_wait(&disp);    //decrementa a disponibilidade
      sem_wait(&mutex);   //entra na zona critica e trava o mutex
     
      int elem_p = rand() % 1000; //produz um inteiro entre 0 e 999
      buffer[cont_p % TAM_BUFFER] = elem_p; //escreve no buffer

      printf("Prod WRITE [%d] = %d\n", cont_p, elem_p);      

      cont_p++;
                        
      sem_post(&mutex);   //sai da regiao critica e libera o mutex
      sem_post(&ocup);    //incrementa a ocupação
    }
    return 0;
}

void *fn_cons(void * arg){
  int cont_c = 0; //o contador interno do consumidor
  while (!fim) {
      sem_wait(&ocup);   //decrementa a ocupação
      sem_wait(&mutex);  //entra na zona critica e trava o mutex
            

      int elem_c = buffer[cont_c % TAM_BUFFER]; //consome um elemento
      printf("Cons READ [%d] = %d\n", cont_c, elem_c);   

      cont_c++;          
  
      if (cont_c == (TAM_BUFFER-1)) fim = 1; //sinaliza o fim
      
      sem_post(&mutex);  //sai da zona critica e libera o mutex
      sem_post(&disp);   //incrementa a disponibilidade
  }
  return 0;
}
