
/*****************************************************************************
 *                                                                           *
 * This sample program demonstrates the use of SGI high resolution timers    *
 * in SGI REACT.                                                             *
 *                                                                           *
 * A simple way to build this sample program is:                             *
 *   cc -o timer_sample timer_sample.c -lrt                                  *
 *                                                                           *
 * Invocation example (500 usec timer):                                      *
 *   ./timer_sample 500                                                      *
 *                                                                           *
 * Invocation example (500 usec timer on realtime cpu 2):                    *
 *   cpuset --invoke=/rtcpu2 --invokecmd=./timer_sample 500                  *
 *                                                                           *
 *****************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <asm/unistd.h>
#include <pthread.h>
#include <strings.h>
#include <sys/time.h>
#include <getopt.h>
#include <libgen.h>

struct timespec time1;
int flag;

/* Timer has triggered, get current time and indicate completion */
void sigalarm(int signo)
{
        clock_gettime(CLOCK_REALTIME,&time1);
        flag = 1;
}


int timer_test(int clock_id, long nanosec) {
        struct itimerspec ts;
        struct sigevent se;
        struct sigaction act;
        sigset_t sigmask;
        struct timespec sleeptime, time0;
        timer_t timer_id;
        long i;
        int signum = SIGRTMAX;
        int status;

        /* Set up sleep time for loops: */
        sleeptime.tv_sec = 1;
        sleeptime.tv_nsec = 0;

        /* Set up signal handler: */
        sigfillset(&act.sa_mask);
        act.sa_flags = 0;
        act.sa_handler = sigalarm;
        sigaction(signum, &act, NULL);

        /* Set up timer: */
        memset(&se, 0, sizeof(se));
        se.sigev_notify = SIGEV_SIGNAL;
        se.sigev_signo = signum;
        se.sigev_value.sival_int = 0;
        status = timer_create(clock_id, &se, &timer_id);
        if (status < 0) {
                perror("timer_create");
                return -1;
        }


        /* Start relative timer: */
        ts.it_value.tv_sec = nanosec / 1000000000;
        ts.it_value.tv_nsec = (nanosec % 1000000000);
        ts.it_interval.tv_sec = 0;
        ts.it_interval.tv_nsec = 0;

        printf("Waiting for timeout of relative timer: ");
        fflush(stdout);
        flag = 0;
        /* Get current time for reference */
        clock_gettime(CLOCK_REALTIME,&time0);
        /*
         * There will be some latency between getting the start time above,
         * and setting the relative time in timer_settime.
         */
        status = timer_settime(timer_id, 0, &ts, NULL);
        if (status < 0) {
                perror("timer_settime");
                return -1;
        }

        /* Loop waiting for timer to go off */
        while (!flag) nanosleep(&sleeptime, NULL);
        if (time1.tv_nsec < time0.tv_nsec)
                printf("Total time=%luns\n",
                        1000000000LL - (time0.tv_nsec - time1.tv_nsec) +
                        ((time1.tv_sec - time0.tv_sec -1)*1000000000LL));
        else
                printf("Total time=%luns\n",
                        time1.tv_nsec - time0.tv_nsec +
                        ((time1.tv_sec - time0.tv_sec)*1000000000LL));


        /* Start absolute timer: */
        printf("Waiting for timeout of absolute timer: ");
        fflush(stdout);
        flag = 0;
        /* Get current time and add timeout to that for absolute time */
        clock_gettime(CLOCK_REALTIME,&time0);
        i = time0.tv_nsec + (nanosec % 1000000000);
        ts.it_value.tv_nsec = i % 1000000000;
        ts.it_value.tv_sec = (time0.tv_sec + (nanosec / 1000000000)) +
                        (i / 1000000000);
        /* There should be less latency than what we saw above */
        status = timer_settime(timer_id, TIMER_ABSTIME, &ts, NULL);
        if (status < 0) {
                perror("timer_settime");
                return -1;
        }

        /* Loop waiting for timer to go off */
        while (!flag) nanosleep(&sleeptime, NULL);
        if (time1.tv_nsec < time0.tv_nsec)
                printf("Total time=%luns\n",
                        1000000000LL - (time0.tv_nsec - time1.tv_nsec) +
                        ((time1.tv_sec - time0.tv_sec -1)*1000000000LL));
        else
                printf("Total time=%luns\n",
                        time1.tv_nsec - time0.tv_nsec +
                        ((time1.tv_sec - time0.tv_sec)*1000000000LL));


        /* Cleanup */
        timer_delete(timer_id);

        return 0;
}

int main(int argc, char *argv[])
{
        long timeout;

        if (argc < 2) {
                printf("usage: %s <timeout usec>\n", basename(argv[0]));
                return -1;
        }

        timeout = atol(argv[1]);
        if (timeout <= 0) {
                printf("Timeout negative or 0 specified\n");
                printf("usage: %s <timeout usec>\n", basename(argv[0]));
                return -1;
        }

        /* Run timer_test with high resolution timer. */
        printf("\nRunning with CLOCK_REALTIME (normal resolution)..\n");
        if (timer_test(CLOCK_REALTIME, timeout * 1000)) {
                return -1;
        }
}
