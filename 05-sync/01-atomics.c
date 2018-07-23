/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Isaac Maia Pessoa
 *
 * Descrição:
 * Instruções Atômicas
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


/*
 * Instruções atômicas: Executadas de forma INDIVISÍVEL e 
 * ININTERRUPTA pelo processador
 * As funções abaixo usam 'built-ins' do gcc que são funções
 * providas APENAS pelo gcc
 * Essas built-ins são chamadas assembly diretas à instruções
 * atômicas do processador
 */



/*
 *  TEST_AND_SET( ou __sync_lock_test_and_set) é uma instrução usada para
 *  modificar atomicamente um endereço de memória com um dado valor 
 *  que é normalmente 1
 *  
 *  O comportamento função pode ser representada pelo algorítmo abaixo:
 *
 *  bool TEST_AND_SET(bool * trava) {
 *      bool valor_antigo = trava
 *      trava = 1
 *      return valor_antigo
 *  }
 *  
 *  O algorítmo acima é executado de forma ATÔMICA pela CPU
 *
*/
#define TEST_AND_SET(ptr, valor)           __sync_lock_test_and_set(ptr,valor)




/*
 * COMPARE_AND_SWAP( ou __sync_val_compare_and_swap) é uma instrução atômica
 * usada para modificar condicionamente e atomicamente um endereço de memória.
 * A condição para a modificação  é que o valor atual do endereço seja igual ao 
 * valor passado como parâmetro da instrução
 * 
 * O comportamento de CAS é dado pelo algorítmo abaixo
 * int CAS(ptr, valor_cmp, novo_valor) {
 *   int temp = *ptr //guarda o valor atual do endereço
 *   if (*ptr == valor_cmp) 
 *     *ptr = novo_valor
 *   return temp
 * }
 */
#define COMPARE_AND_SWAP(ptr,antigo,novo)  __sync_val_compare_and_swap(ptr, antigo, novo)



/*
 * A instrução atômica  __sync_lock_release serve para escrever atômicamente
 * o valor 0 em um dado endereço
 */
#define DESTRAVA(ptr)                      __sync_lock_release(ptr)



//trava o 'cadeado' para acesso exclusivo
void trava (int *ptr);

//destrava o 'cadeado' para outras threads poderem avançar
void destrava (int *ptr);

void *th1_func (void *arg);
void *th2_func (void *arg);


int global_var;			//essa variável será modificada por duas threads
int cadeado;			//essa variável servirá de 'trava' para garantir acesso exclusivo à variável global


int main (void)
{
  pthread_t thread1;
  pthread_t thread2;

  global_var = 0;

  pthread_create (&thread1, NULL, th1_func, NULL);
  pthread_create (&thread2, NULL, th2_func, NULL);

  pthread_join (thread1, NULL);
  pthread_join (thread2, NULL);

  return 0;
}



void * th1_func (void *arg)
{
  for (int i = 0; i < 100; i++)
    {
      trava (&cadeado);		//pede permissão para entrar na região crítica      

      //inicio da região crítica
      global_var++;
      printf ("Thread 1, global_var++ = %d\n", global_var);
      //fim da região crítica

      destrava (&cadeado);	//libera a região crítica para outra thread
    }
  return 0;
}


void * th2_func (void *arg)
{
  for (int i = 0; i < 100; i++)
    {
      trava (&cadeado);		//pede permissão para entrar na região crítica

      //inicio da região crítica
      global_var--;
      printf ("Thread 2, global_var-- = %d\n", global_var);
      //fim da região crítica

      destrava (&cadeado);	//libera a região crítica para outra thread
    }
  return 0;
}


void trava (int *ptr)
{
  //espera até que o valor retornado por TEST_AND_SET seja 0
  while (TEST_AND_SET (ptr, 1) == 1)
                 ; //fica rodando sem nada fazer (spin)
}

void destrava (int *ptr)
{
  DESTRAVA(ptr);
}
