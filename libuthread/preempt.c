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
	/* TODO Phase 4 */
}

/*
 * preempt_enable - Enable preemption
 */
void preempt_enable(void)
{
	/* TODO Phase 4 */
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
void alarm_handler(int signum)
{
    uthread_yield();
}

void preempt_start(bool preempt)
{
	struct sigaction new_action, old_action;

    /* Set up handler for alarm */
    new_action.new_action_handler = alarm_handler;
    sigemptyset(&new_action.new_action_mask);
    new_action.new_action_flags = 0;
    sigaction(SIGVTALRM, &new_action, &old_action);

    /* Configure alarm */
    struct itimerval it_val;
    it_val.it_value.tv_sec = 0;
    it_val.it_value.tv_usec = 1000000 / HZ;
    it_val.it_interval = it_val.it_value;
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        perror("error calling setitimer()");
        exit(1);
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
	/* TODO Phase 4 */
}



