#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <semaphore.h>

#define REGION_NAME "ksjahnkxljsahdfksxajm"
struct shared_memory {
	sem_t mutex;
	sem_t contains_nothing;
	sem_t contains_lowercase;
	sem_t contains_uppercase;
	char data[1024];
};

int shared_memory_open(int *fd, struct shared_memory **smp, int initialize);
void shared_memory_close(int *fd, struct shared_memory **smp, int initialized);

#endif
