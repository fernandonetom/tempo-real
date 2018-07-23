/* Federal University of Paraiba - UFPB
 * Department o Electrical Engineering - DEE
 * Real Time Systems
 *
 * (c) 2016 Lucas V. Hartmann <lhartmann@github.com>
 *
 * Posix message queues example code: Reader (receiver).
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>

#define MQ_MSG_SIZE 1024
#define MQ_NAME_UC "/dfkjhasdjkflxa_uc"
#define MQ_NAME_LC "/dfkjhasdjkflxa_lc"

int main() {
	// Open a message queue, and make sure it worked.
	int mqfd;
	mqfd = mq_open(MQ_NAME_UC, O_RDONLY);
	if (mqfd == -1) {
		printf("Failed to open UC message queue.\n");
		return 1;
	}
	
	// Enter the message loop
	int keep_looping = 1;
	while (keep_looping) {
		// Receive a message
		char msg[MQ_MSG_SIZE+2];
		ssize_t sz = mq_receive(mqfd, msg, MQ_MSG_SIZE+1, 0);
		
		// Handle errors
		if (sz == -1) {
			printf("mq_receive failed (errno=%d).\n", errno);
			break;
		}
		
		// No message writers 
		if (sz == 0) break;
		
		// Restore NULL-terminator
		msg[sz] = 0;
		
		// Print the message
		printf("GOT: \"%s\"\n", msg);
		
		// If the message is "END" then stop looping
		if (strcmp(msg, "END") == 0) keep_looping = 0;
	}

	// Close the shared memory region.
	mq_close(mqfd);

	// All went well, let the OS know it.
	return 0;
}
