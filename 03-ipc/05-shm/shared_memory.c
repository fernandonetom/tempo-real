#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "shared_memory.h"

#if _POSIX_C_SOURCE < 200112L
static int ftruncate(int fd, off_t sz) {
	for (int i=0; i<(sz); ++i) write(fd, "\0", 1);
}
#endif

int shared_memory_open(int *fd, struct shared_memory **smp, int initialize) {
	// Creates a file handle to a shared memory region
	if (initialize) {
		// If we are initializing the region we have to create the file
		printf("Creating a shared memory region...\n");
		*fd = shm_open(REGION_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	} else {
		// If not initializing the file must exist (no O_CREAT).
		printf("Opening a shared memory region...\n");
		*fd = shm_open(REGION_NAME, O_RDWR, S_IRUSR | S_IWUSR);
	}
	
	// shm_open returns -1 on error
	if (*fd == -1) {
		printf("shm_open failed.\n");
		return -1;
	}
	
	// If we are creating the shared memory we have to set it's size
	if (initialize) {
		int r = ftruncate(*fd, sizeof(struct shared_memory));
		if (r == -1) {
			printf("ftruncate failed\n");
			return -1;
		}
	}
	
	// Map the shared region on this process's virtual memory space.
	// Search opengroup to find what the parameters mean
	void *vptr = mmap(
		0,                            // ?
		sizeof(struct shared_memory), // ?
		PROT_READ|PROT_WRITE,         // ?
		MAP_SHARED,                   // ?
		*fd,                          // ?
		0                             // ?
	);
	
	// mmap returns MAP_FAILED on error.
	if (vptr == MAP_FAILED) {
		printf("shm_open failed.\n");
		return -1;
	}
	
	// Change pointer type to match our messages
	*smp = (struct shared_memory *)vptr;
	
	// Initialize the shared semaphores
	if (initialize) {
		sem_init(&(*smp)->mutex,              1, 1); // Mutex starts unlocked
		sem_init(&(*smp)->contains_nothing,   1, 1); // Buffer starts empty
		sem_init(&(*smp)->contains_lowercase, 1, 0);
		sem_init(&(*smp)->contains_uppercase, 1, 0);
	}
	
	// All went well	
	return 0;
}

void shared_memory_close(int *fd, struct shared_memory **smp, int initialized) {
	// Destroy semaphores if we created them
	if (initialized) {
		sem_destroy(&(*smp)->mutex);
		sem_destroy(&(*smp)->contains_nothing);
		sem_destroy(&(*smp)->contains_lowercase);
		sem_destroy(&(*smp)->contains_uppercase);
	}
	
	// Release memory mapping
	munmap(*smp, sizeof(struct shared_memory));

	// Close and invalidate memory handle
	close(*fd);
	*fd = -1;
	
	// Remove the shared region if we created it
	if (initialized) shm_unlink(REGION_NAME);
}
