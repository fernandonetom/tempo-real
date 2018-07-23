/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição: 
 * Sincronização entre threads com mutexes
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



//variável compartilhada entre as threads
int global_var;

//declara o mutex e o inicializa
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *th1_func(void * param); 
void *th2_func(void * param); 

 
int main(void)  
{
  pthread_t thread1;
  pthread_t thread2;
  
  pthread_create(&thread1,NULL,th1_func,NULL); //cria a thread 1 passando a função como argumento
  pthread_create(&thread2,NULL,th2_func,NULL); //cria a thread 2 passando a mesma função como argumento

  pthread_join(thread1,NULL); 
  pthread_join(thread2,NULL); 
  return 0;
}



void *th1_func(void * param)
{
  for (int i=0;i<100;i++)
  {
      //região livre de disputa
      //nessa região, a thread pode fazer algum trabalho sem necessidade 
      //de sincronização     
      pthread_mutex_lock(&mutex); //pede permissão para entrar na região crítica
      global_var++;
      printf ("Thread 1, global_var++ = %d\n", global_var);
      pthread_mutex_unlock(&mutex); //libera a região crítica para outra thread

      //região livre de disputa
  }
  return 0;
}



void *th2_func(void * param)
{
  for (int i=0;i<100;i++)
  {
      pthread_mutex_lock(&mutex); //pede permissão para entrar na região crítica

      //inicio da região crítica
      global_var--;
      printf ("Thread 2, global_var-- = %d\n", global_var);
      //fim da região crítica

      pthread_mutex_unlock(&mutex); //libera a região crítica para outra thread
  }
  return 0;
}
