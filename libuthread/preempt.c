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

/*
 * preempt_dinew_actionble - Dinew_actionble preemption
 */
void preempt_disable(void)
{
    sigemptyset(&block_alarm);
    sigaddset(&block_alarm, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &block_alarm, NULL);
}

/*
 * preempt_enable - Enable preemption
 */
void preempt_enable(void)
{
    sigemptyset(&block_alarm);
    sigaddset(&block_alarm, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &block_alarm, NULL);
}

void alarm_handler(__attribute__((unused)) int signum)
{
    uthread_yield();
}

/*
 * preempt_start - Start thread preemption
 * @preempt: Enable preemption if true
 *
 * Configure a timer that must fire a virtual alarm at a frequency of 100 Hz and
 * setup a timer handler that forcefully yields the currently running thread.
 *
 * If @preempt is false, don't start preemption; all the other functions from
 * the preemption API should then be ineffective.
 */
void preempt_start(bool preempt)
{
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
        if (setitimer(ITIMER_VIRTUAL, &new_timer, NULL) == -1) {
            perror("fail to call setitimer()\n");
            exit(1);
        }
    }
}

/*
 * preempt_stop - Stop thread preemption
 *
 * Restore previous timer configuration, and previous action associated to
 * virtual alarm signals.
 */
void preempt_stop(void)
{
    // restore timer
    if (setitimer(ITIMER_VIRTUAL, &old_timer, NULL) == -1) {
        perror("fail to call setitimer()\n");
        exit(1);
    }

    //resotre action
    sigemptyset(&old_action.sa_mask);
    sigaction(SIGVTALRM, &old_action, NULL);
}
