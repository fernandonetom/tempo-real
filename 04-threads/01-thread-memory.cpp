/* Department of Electrical Engineering - DEE
 * Alternative and Renewable Energy Center - CEAR
 * Federal university of Parahyba - UFPB
 * 
 * Real-time Systems
 * 
 * Lucas Vinicius Hartmann
 *
 * Description: Thread example, covering IDs, join,
 * and thread-locad verus process-global memory. 
 */

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

int global_var = 5;

// This time we use arg as a C-string:
void *thread_function(void *arg) {
	char *str = (char*) arg;

	int local_var = 0;

	for (int i=0; i<10; i++) {
		usleep(1000000);
		local_var++;
		global_var++;
		cout 
			<< "TID=" << pthread_self()
			<< ", arg=" << str 
			<< ": pid=" << getpid()
			<< ", ppid=" << getppid()
			<< ", global=" << global_var
			<< ", local_var=" << local_var << endl;
	}
	return 0;
}

int main() {
	pthread_t t0, t1;

	cout << "Main thread ID: " << pthread_self() << endl;
	pthread_create(&t0, 0, thread_function, (void*)"A");
	cout << "Created t0, ID: " << t0 << endl;
	usleep(500000);
	pthread_create(&t1, 0, thread_function, (void*)"B");
	cout << "Created t1, ID: " << t1 << endl;

	cout << "Main waiting for t0..." << endl;
	pthread_join(t0, 0);
	cout << "Main waiting for t1..." << endl;
	pthread_join(t1, 0);
	cout << "Done!" << endl;
	return 0;
}
