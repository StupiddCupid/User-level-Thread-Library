#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction new_action, old_action;
struct itimerval new_timer, old_timer;
sigset_t block_alarm;

void preempt_disable(void)
{
    // use block alarm to mask the alarm signal
    sigemptyset(&block_alarm);
    sigaddset(&block_alarm, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &block_alarm, NULL);
}

void preempt_enable(void)
{
    sigemptyset(&block_alarm);
    sigaddset(&block_alarm, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &block_alarm, NULL);
}

void alarm_handler(int signum)
{
    (void)signum;
    uthread_yield();
}

void preempt_start(bool preempt)
{
    // if preempt is true, initializes timer and signal handler
    // also stores old signal action and old timer configuration
    if (preempt) {
        /* Set up handler for alarm */
        new_action.sa_handler = alarm_handler;
        new_action.sa_flags = 0;
        sigemptyset(&new_action.sa_mask);
        sigaction(SIGVTALRM, &new_action, &old_action);
    
        /* set up alarm */
        new_timer.it_value.tv_sec = 0;
        new_timer.it_value.tv_usec = 1000000 / HZ;
        new_timer.it_interval = new_timer.it_value;
        setitimer(ITIMER_VIRTUAL, &new_timer, &old_timer);   
    }
}

void preempt_stop(void)
{
    // restore timer
    setitimer(ITIMER_VIRTUAL, &old_timer, NULL);

    // resotre action
    sigemptyset(&old_action.sa_mask);
    sigaction(SIGVTALRM, &old_action, NULL);
}
