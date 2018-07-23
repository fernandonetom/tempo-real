/* Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * (c) 2015 Isaac Maia Pessoa
 * (c) 2016 Lucas Vinicius Hartmann
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
#include <time.h>
#include <sched.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>
#include "../../fakebusy.h"

// Define a data structure to be shared amongst threads
typedef struct {
	pthread_mutex_t mutex;
	sem_t semLow;
	sem_t semAvg;
	sem_t semHigh;
	
	int die;
} lockset_t;

// The three threads of this program
void *high_priority_thread(lockset_t *ls);
void *average_priority_thread(lockset_t *ls);
void *low_priority_thread(lockset_t *ls);

// The type 
typedef void *(*thread_fcn)(void*);

int main(void) {
	// Shared locks between threads
	lockset_t ls;
	ls.die = 0;
	sem_init(&ls.semLow,  0, 0);
	sem_init(&ls.semAvg,  0, 0);
	sem_init(&ls.semHigh, 0, 0);
	pthread_mutex_init(&ls.mutex, 0);
	
	// Identifiers and attribute structures
	pthread_t      tid_hi, tid_avg, tid_low;
	pthread_attr_t attr;
	struct sched_param sp;
	
	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	
	// High-priority thread:
	sp.sched_priority = sched_get_priority_max(SCHED_FIFO) - 10;
	pthread_attr_setschedparam(&attr, &sp);
	pthread_create(&tid_hi, &attr, (thread_fcn)high_priority_thread, &ls);
	
	// Average-priority thread:
	sp.sched_priority = sched_get_priority_max(SCHED_FIFO) - 20;
	pthread_attr_setschedparam(&attr, &sp);
	pthread_create(&tid_avg, &attr, (thread_fcn)average_priority_thread, &ls);
	
	// Low-priority thread:
	sp.sched_priority = sched_get_priority_max(SCHED_FIFO) - 30;
	pthread_attr_setschedparam(&attr, &sp);
	pthread_create(&tid_low, &attr, (thread_fcn)low_priority_thread, &ls);
	
	// Generate some excitement for the threads:
	for (int i=0; i<50; ++i) {
		sem_post(&ls.semLow);
		usleep(100000);
	}
	
	// Signal the threads to end
	ls.die = 1;
	sem_post(&ls.semLow);
	sem_post(&ls.semAvg);
	sem_post(&ls.semHigh);
	
	// Join
	pthread_join(tid_hi,  0);
	pthread_join(tid_avg, 0);
	pthread_join(tid_low, 0);
	usleep(1000);
	
	return 0;
}

#define BUSY_WAIT() {               \
	volatile unsigned long ctr = 1e9; \
	while (ctr--);                    \
}

void *high_priority_thread(lockset_t *ls) {
	while (1) {
		sem_wait(&ls->semHigh);
		if (ls->die) break;
		
		BUSY_WAIT();
		
		pthread_mutex_lock(&ls->mutex);
		
		BUSY_WAIT();
		BUSY_WAIT();
		
		pthread_mutex_unlock(&ls->mutex);
		
		BUSY_WAIT();
	}
	return 0;
}

void *average_priority_thread(lockset_t *ls) {
	while (1) {
		sem_wait(&ls->semAvg);
		if (ls->die) break;
		
		BUSY_WAIT();
		BUSY_WAIT();
		
		sem_post(&ls->semHigh);
		
		BUSY_WAIT();
		BUSY_WAIT();
	}
	return 0;
}

void *low_priority_thread(lockset_t *ls) {
	while (1) {
		sem_wait(&ls->semLow);
		if (ls->die) break;
		
		BUSY_WAIT();
		
		pthread_mutex_lock(&ls->mutex);
		
		BUSY_WAIT();
		
		sem_post(&ls->semAvg);
		
		BUSY_WAIT();
		
		pthread_mutex_unlock(&ls->mutex);
		
		BUSY_WAIT();
	}
	return 0;
}
