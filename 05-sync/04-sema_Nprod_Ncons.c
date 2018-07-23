/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição: 
 * Semáforos: Vários Produtores e Vários Consumudor
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


#define TAM_BUFFER  1000
int     buffer[TAM_BUFFER];

sem_t ocup;  //semaforo para inidicar a ocupação/preenchimento do buffer
sem_t disp;  //semaforo para indicar a disponibilidade de vagas no buffer
sem_t mutex; //semaforo para a região crítica/ podemos usar um mutex_t no lugar de sem_t
int   fim;   //sinaliza o fim da execução


int fim;           //sinaliza o fim da execução
int cont_c = 0;    //contador compartilhado entre todos os consumidores
int cont_p = 0;    //contador comparitlhado entre todos os produtores


int main(int argc, char ** argv)  
{
  //numero de threads a ser criados n/2 produtores e n/2 consumidores
  int n_thds = atoi(argv[1]);
  
  if ((n_thds % 2) != 0) {
	  perror("Entre com um argumento multiplo de 2\n");
	  exit(0);
  }
  if (n_thds < 2) n_thds = 2;
	
  pthread_t th_prod[n_thds/2];
  pthread_t th_cons[n_thds/2];
  int       th_id[n_thds];
    
  
  //preenche o buffer com zeros
  memset(buffer, 0 , TAM_BUFFER*sizeof(int));


  //inicia os semáforos, o segundo argumento (0) indica se o semáforo será compartilhado entre processos ou threads
  sem_init(&ocup,0,0);            //inicialmente o buffer está vazio, iniciamos ocup com 0 pois não existe ocupação no mesmo
  sem_init(&disp,0,TAM_BUFFER);   //O número de vagas disponíveis no buffer
  sem_init(&mutex,0,1);           //Isso é um semáforo binário para proteger a região crítica

  //inicializa a semente randômica
  srand (time(NULL) );
  
  //inicializa a flag
  fim = 0;
  
  //cria pares de consumidor/produtor
  for(int i=0; i < (n_thds/2) ; i++) {	  
     th_id[i] = i;
     pthread_create(&th_prod[i],NULL,fn_prod, &th_id[i]);
     pthread_create(&th_cons[i],NULL,fn_cons, &th_id[i]);
  }
    
  for(int i=0; i < (n_thds/2) ; i++) {	  
     pthread_join(th_prod[i],NULL); 
     pthread_join(th_cons[i],NULL); 	 
  }

  return 0;
}


void *fn_prod(void * arg){
    int *meu_id = (int *)arg;	
    while (!fim){
      sem_wait(&disp);    //decrementa a disponibilidade
      sem_wait(&mutex);   //entra na zona critica e trava o mutex
     
      int elem_p = rand() % 1000; //produz um inteiro entre 0 e 999
      buffer[cont_p % TAM_BUFFER] = elem_p; //escreve no buffer

      printf("Prod [%d] : WRITE [%d] = %d\n", *meu_id, cont_p, elem_p);

      cont_p++;
                        
      sem_post(&mutex);   //sai da regiao critica e libera o mutex
      sem_post(&ocup);    //incrementa a ocupação
    }
    return 0;
}



void *fn_cons(void * arg){
  int *meu_id = (int *)arg;
  while (!fim) {
      sem_wait(&ocup);   //decrementa a ocupação
      sem_wait(&mutex);  //entra na zona critica e trava o mutex
            

      int elem_c = buffer[cont_c % TAM_BUFFER]; //consome um elemento
      printf("Cons [%d] : READ [%d] = %d\n", *meu_id, cont_c, elem_c);   

      cont_c++;          
  
      if (cont_c == (TAM_BUFFER-1)) fim = 1; //sinaliza o fim
      
      sem_post(&mutex);  //sai da zona critica e libera o mutex
      sem_post(&disp);   //incrementa a disponibilidade
  }
  
  return 0;
}
