/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição: 
 * Sincronização entre threads com spinlocks
 *
 * Compilação: 
 * gcc -std=c99 -o prog prog.c -lpthread
 *
 * OBS: substituir 'prog' pelo nome do arquivo
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


//variável compartilhada entre as threads
int global_var;

//declara o spinlock
//os spinlocks ficam em laço de espera oculpada(busy waiting)
//sem sair da CPU, até que o lock seja liberado
pthread_spinlock_t  trava;

void *th1_func(void * param); 
void *th2_func(void * param); 

 
int main(void)  
{
  pthread_t thread1;
  pthread_t thread2;

  //cria um lock (trava) a ser usada apenas pelas threads desse processo
  pthread_spin_init(&trava, PTHREAD_PROCESS_PRIVATE);
  
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
      pthread_spin_lock(&trava); //pede permissão para entrar na região crítica
      global_var++;
      printf ("Thread 1, global_var++ = %d\n", global_var);
      pthread_spin_unlock(&trava); //libera a região crítica para outra thread

	  usleep(1);
      //região livre de disputa
  }
  return 0;
}



void *th2_func(void * param)
{
  for (int i=0;i<100;i++)
  {
      pthread_spin_lock(&trava); //pede permissão para entrar na região crítica

      //inicio da região crítica
      global_var--;
      printf ("Thread 2, global_var-- = %d\n", global_var);
      //fim da região crítica

      pthread_spin_unlock(&trava); //libera a região crítica para outra thread

	  usleep(1);
  }
  return 0;
}
