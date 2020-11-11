/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Lucas Vinicius Hartmann
 *
 * Descrição:
 * Instruções Atômicas
 */
#include <iostream>
#include <pthread.h>
#include <math.h>

using namespace std;

/* Defines runner_t as the type of a thread function, so we may cast the
 * function during pthread_create, instead of the argument on the
 * function. It changes nothing, just another option.
 */
typedef void* (*runner_t)(void*);

typedef struct arguments_s {
	int N;
	int count;
	int *lock; // NEW: pointer to a shared lock
} arguments_t;

// Defines a thread function, but with appropriate arg pointer type.
void *runner(arguments_t *arg) {
	int i;
	for (i=0; i<arg->N; ++i) {
		// __sync_lock_test_and_set will put 1 on the lock,
		// and return the old value, atomically.
		while (__sync_lock_test_and_set(arg->lock, 1)) {
			// Old value was not-zero, meaning locked.
			// try again.
		}
		// Old value was zero, meaning unlocked.
		// We wrote 1, so it is locked for everyone else.

		// count++ in a way that takes some cpu time.
		arg->count = sqrt((arg->count+1.)*(arg->count+1.));

		// Write zero again to unlock.
		__sync_lock_test_and_set(arg->lock, 0);
	}
	return 0;
}

// MAIN
#define NUM_THREADS 100
int main() {
	pthread_t id[NUM_THREADS];
	int i;
	int lock = 0; // NEW: The shared lock (0=open).
	arguments_t arg;
	arg.N = 10000;
	arg.count = 0;
	arg.lock = &lock; // NEW
	
	// Spawn threads
	for (i=0; i<NUM_THREADS; ++i) {
		pthread_create(&id[i], 0, (runner_t)runner, &arg);
	}
	
	// Join threads (wait for them to complete)
	for (i=0; i<NUM_THREADS; ++i) {
		pthread_join(id[i], 0);
	}
	
	// Print the final count
	cout << "Final count = " << arg.count << endl;
	return 0;
}
