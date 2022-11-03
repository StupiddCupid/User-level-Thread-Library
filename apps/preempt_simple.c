/*
 * preempt
 *
 * Tests if current thread is changed every 1000000/100 seconds
 *
 * thread1
 * thread2
 * thread3
 */

#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <uthread.h>

static struct timeval t1, t2, t3;
static double seconds;
static int counter;

void thread3(void *arg)
{
	(void)arg;

	while(counter < 500000) {
		gettimeofday(&t3, NULL);
		seconds = (double)(t3.tv_usec - t2.tv_usec) / 1000000;
		printf("In thread3, time passed: %f\n", seconds);
		counter++;
	}
	printf("end at thread3\n");
}

void thread2(void *arg)
{
	(void)arg;
	while(counter < 500000) {
		gettimeofday(&t2, NULL);
		seconds = (double)(t2.tv_usec - t1.tv_usec) / 1000000;
		printf("In thread2, time passed: %f\n", seconds);
		counter++;
	}
	printf("end at thread2\n");
}

void thread1(void *arg)
{
	(void)arg;
	uthread_create(thread2, NULL);
	uthread_create(thread3, NULL);
	while(counter < 500000) {
		gettimeofday(&t1, NULL);
		seconds = (double)(t1.tv_usec - t3.tv_usec) / 1000000;
		printf("In thread1, time passed: %f\n", seconds);
		counter++;
	}
	printf("end at thread1\n");
}

int main(void)
{
	counter = 0;
	printf("---------------TESTING PREEMPT_SIMPLE---------------\n");
	uthread_run(true, thread1, NULL);
	printf("-----------------END PREEMPT_SIMPLE-----------------\n");
	return 0;
}