#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>

using namespace std;

struct data_t {
	sem_t s;
};

void parent(data_t *d) {
}

void child(data_t *d) {
}

extern int errno;

#define ASSERT(cond, errormsg) {  \
	if (!(cond)) {                \
		cout << "Assertion (" #cond ") failed on line "; \
		cout << __LINE__ << ": errno=" << errno << ",  " << errormsg << endl; \
		return 1;                 \
	}                             \
}

int main() {
	// Create a shared memory block
	void *shm = mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, -1, 0);
	ASSERT(shm != MAP_FAILED, "mmap failed.");
	
	// Change the pointer type
	data_t *data = (data_t *)shm;
	
	// Initialize
	if (sem_init(&data->s, 1, 1) == -1) {
		cout << "sem_init failed" << endl;
		return 1;
	}
}

