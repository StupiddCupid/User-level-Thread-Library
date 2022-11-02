#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define HZ 100

static int counter = 1;
static time_t t;


void alarm_handler(int signum)
{
    time(&t);
    printf("counter value: %d ", counter++);
    printf("time: %s", ctime(&t));
    if(counter == 10) {
        clock_t end = clock() - begin;
        double timetaken = ((double)t) / CLOCKS_PER_SEC;
        printf("alarm handler took %f seconds\n", timetaken);
        raise(SIGSTOP);
    }
}

int main(void)
{
    struct sigaction sa;
    // sigset_t ss;

    /* Set up handler for alarm */
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    /* Configure alarm */
    struct itimerval it_val;

    it_val.it_value.tv_sec = 1;
    it_val.it_value.tv_usec = 0;
    it_val.it_interval = it_val.it_value;
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        perror("error calling setitimer()");
        exit(1);
    }

    begin = clock();
    while(1) {
        pause();
    }

    return 0;
}