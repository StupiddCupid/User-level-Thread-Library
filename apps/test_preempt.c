/*
 * preempt
 *
 * Tests if current thread is changed every 1/100 seconds
 *
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

static int counter;

void thread3(void *arg)
{
	(void)arg;
	double seconds3 = 0.0;
	struct timeval start3, t3;
	gettimeofday(&start3, NULL);
	while(counter < 10) {
		while(seconds3 < 0.0095) {
			gettimeofday(&t3, NULL);
			seconds3 = (double)(t3.tv_usec - start3.tv_usec) / 1000000;
		}
		printf("In thread3\n");
		gettimeofday(&start3, NULL);
		seconds3 = 0.0;
		counter += 1;
	}
	printf("End of thread3\n");
}

void thread2(void *arg)
{
	(void)arg;
	double seconds2 = 0.0;
	struct timeval start2, t2;
	gettimeofday(&start2, NULL);
	while(counter < 10) {
		while(seconds2 < 0.0095) {
			gettimeofday(&t2, NULL);
			seconds2 = (double)(t2.tv_usec - start2.tv_usec) / 1000000;
		}
		printf("In thread2\n");
		gettimeofday(&start2, NULL);
		seconds2 = 0.0;
		counter += 1;
	}
	printf("End of thread2\n");
}

void thread1(void *arg)
{
	(void)arg;
	uthread_create(thread2, NULL);
	uthread_create(thread3, NULL);

	double seconds1 = 0.0;
	struct timeval start1, t1;
	gettimeofday(&start1, NULL);
	while(counter < 10) {
		while(seconds1 < 0.009) {
			gettimeofday(&t1, NULL);
			seconds1 = (double)(t1.tv_usec - start1.tv_usec) / 1000000;
		}
		printf("In thread1\n");
		gettimeofday(&start1, NULL);
		seconds1 = 0.0;
		counter += 1;
	}
	printf("End of thread1\n");
}

int main(void)
{
	counter = 0;
	printf("---------------TESTING PREEMPT_SIMPLE---------------\n");
	uthread_run(true, thread1, NULL);
	printf("-----------------END PREEMPT_SIMPLE-----------------\n");
	return 0;
}