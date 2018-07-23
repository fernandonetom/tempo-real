/* Federal University of Paraiba - UFPB
 * Department o Electrical Engineering - DEE
 * Real Time Systems
 *
 * (c) 2016 Lucas V. Hartmann <lhartmann@github.com>
 *
 * Posix message queues example code: Worker.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>

#define MQ_MSG_COUNT 4
#define MQ_MSG_SIZE  1024
#define MQ_NAME_UC "/dfkjhasdjkflxa_uc"
#define MQ_NAME_LC "/dfkjhasdjkflxa_lc"

int main() {
	struct mq_attr mqa;
	memset(&mqa, 0, sizeof(mqa));
	mqa.mq_maxmsg  = MQ_MSG_COUNT;
	mqa.mq_msgsize = MQ_MSG_SIZE;
		
	// Open a message queue, and make sure it worked.
	int ucmq;
	ucmq = mq_open(MQ_NAME_UC, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR, &mqa);
	if (ucmq == -1) {
		printf("Failed to create UC message queue (%d).\n", errno);
		return 1;
	}
	
	// Open a message queue, and make sure it worked.
	int lcmq;
	lcmq = mq_open(MQ_NAME_LC, O_CREAT|O_RDONLY, S_IRUSR|S_IWUSR, &mqa);
	if (lcmq == -1) {
		printf("Failed to create LC message queue.\n");
		return 1;
	}
	
	// Enter the message loop
	int keep_looping = 1;
	while (keep_looping) {
		// Receive a lowercase message:
		char msg[MQ_MSG_SIZE+2];
		ssize_t sz = mq_receive(lcmq, msg, MQ_MSG_SIZE+1, 0);
		
		// Handle errors
		if (sz == -1) {
			printf("mq_receive failed.\n");
			break;
		}
		
		// No message writers 
		if (sz == 0) break;
		
		// Restore NULL-terminator
		msg[sz] = 0;
		
		// Change text to uppercase
		for (char *p=msg; *p; p++) *p = toupper(*p);
		
		// Forward the uppercase message
		mq_send(ucmq, msg, sz, 0);
		
		// If the text is "END" then stop looping
		if (strcmp(msg, "END") == 0) keep_looping = 0;
	}

	// Close and remove the message queues
	mq_close(ucmq);
	mq_unlink(MQ_NAME_UC);
	mq_close(lcmq);
	mq_unlink(MQ_NAME_LC);

	// All went well, let the OS know it.
	return 0;
}
