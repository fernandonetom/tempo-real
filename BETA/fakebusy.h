#include <time.h>

void fakebusy(unsigned long nsec) {
	struct timespec now, end;
	
	// When to end wait
	clock_gettime(CLOCK_MONOTONIC, &end);
	end.tv_nsec += nsec;
	if (end.tv_nsec >= 1000000000) {
		end.tv_nsec -= 1000000000;
		end.tv_sec += 1;
	}
	
	// wait
	do {
		clock_gettime(CLOCK_MONOTONIC, &now);
	} while (now.tv_sec < end.tv_sec || now.tv_sec == end.tv_sec && now.tv_nsec < end.tv_nsec); 
}


