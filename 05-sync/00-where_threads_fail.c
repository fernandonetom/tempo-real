#include <stdio.h>
#include <pthread.h>
#include <math.h>

// Defines runner_t as the type of a thread function.
typedef void* (*runner_t)(void*);

typedef struct arguments_s {
	int N;
	int count;
} arguments_t;

// Defines a thread function, but with appropriate arg pointer type.
void *runner(arguments_t *arg) {
	int i;
	for (i=0; i<arg->N; ++i) {
		// count++ in a way that takes some cpu time.
		arg->count = sqrt((arg->count+1.)*(arg->count+1.));
	}
}

// MAIN
#define NUM_THREADS 100
int main() {
	pthread_t id[NUM_THREADS];
	int i;
	arguments_t arg;
	arg.N = 10000;
	arg.count = 0;
	
	// Spawn threads
	for (i=0; i<NUM_THREADS; ++i) {
		pthread_create(&id[i], 0, (runner_t)runner, &arg);
	}
	
	// Join threads (wait for them to complete)
	for (i=0; i<NUM_THREADS; ++i) {
		pthread_join(id[i], 0);
	}
	
	// Print the final count
	printf("Final count = %d\n", arg.count);
	return 0;
}
