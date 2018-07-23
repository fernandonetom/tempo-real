/* Federal University of Paraiba - UFPB
 * Department o Electrical Engineering - DEE
 * Real Time Systems
 *
 * (c) 2016 Lucas V. Hartmann <lhartmann@github.com>
 *
 * Posix message queues example code: Writer (sender).
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <mqueue.h>

#define MQ_MSG_SIZE 1024
#define MQ_NAME_UC "/dfkjhasdjkflxa_uc"
#define MQ_NAME_LC "/dfkjhasdjkflxa_lc"

int main() {
	// Open a message queue, and make sure it worked.
	int mqfd;
	mqfd = mq_open(MQ_NAME_LC, O_WRONLY);
	if (mqfd == -1) {
		printf("Failed to open LC message queue.\n");
		return 1;
	}
	
	// Enter the message loop
	while (1) {
		// Ask the user for a message
		printf("Type a message or END: ");
		char msg[MQ_MSG_SIZE];
		scanf("%s",msg);
		
		// Find out the message size.
		ssize_t len  = strlen(msg);
		
		// Send the message
		mq_send(mqfd, msg, len, 0);
		
		// If the user entered END then break away from the loop.
		if (strcmp(msg, "END") == 0) break;
	}

	// Close the FIFO and remove it.
	mq_close(mqfd);

	// All went well, let the OS know it.
	return 0;
}
