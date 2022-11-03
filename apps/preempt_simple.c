/*
 * preempt
 *
 * Tests if current thread is changed every 1000000/100 seconds
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

void thread3(void *arg)
{
	(void)arg;

	
	printf("thread3\n");
}

void thread2(void *arg)
{
	(void)arg;

	uthread_create(thread3, NULL);
	
	printf("thread2\n");
}

void thread1(void *arg)
{
	(void)arg;

	uthread_create(thread2, NULL);
	
	printf("thread1\n");
	
}

int main(void)
{
	uthread_run(false, thread1, NULL);
	return 0;
}