#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

//global ptr pointing to the front of the queue
queue_t my_thread_Queue;
static struct uthread_tcb *current_thread;

typedef enum {
	READY,
	RUNNING,
	BLOCKED,
	EXITED
} state;

struct uthread_tcb {
	uthread_ctx_t *context_ptr;
	void *stack_ptr;
	state thread_state;
};

struct uthread_tcb *uthread_current(void)
{
	//return the current thread
	return current_thread;
}

/*
 * uthread_yield - Yield execution
 *
 * This function is to be called from the currently active and running thread in
 * order to yield for other threads to execute.
 */
void uthread_yield(void)
{
	//dequeue the curr(make it wait) and enqueue it into the end of the queue
	void* yield_thread;
	queue_dequeue(my_thread_Queue, yield_thread);
	queue_enqueue(my_thread_Queue, yield_thread);
	current_thread = (struct uthread_tcb *)my_thread_Queue->front; 
	//reset the state
	current_thread->thread_state = 1;

}

/*
 * uthread_exit - Exit from currently running thread
 *
 * This function is to be called from the currently active and running thread in
 * order to finish its execution.
 *
 * This function shall never return.
 */
void uthread_exit(void)
{
	/* TODO Phase 2 */
}


/*
 * uthread_create - Create a new thread
 * @func: Function to be executed by the thread
 * @arg: Argument to be passed to the thread
 *
 * This function creates a new thread running the function @func to which
 * argument @arg is passed.
 *
 * Return: 0 in case of success, -1 in case of failure (e.g., memory allocation,
 * context creation).
 */

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb *myThread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	myThread -> stack_ptr = uthread_ctx_alloc_stack();
	myThread -> thread_state = READY;
	int retval = uthread_ctx_init(myThread->context_ptr, myThread->stack_ptr, func, arg);
	if (myThread->stack_ptr == NULL || retval == 0) return -1;
	else return 0;
}

/*
 * uthread_run - Run the multithreading library
 * @preempt: Preemption enable
 * @func: Function of the first thread to start
 * @arg: Argument to be passed to the first thread
 *
 * This function should only be called by the process' original execution
 * thread. It starts the multithreading scheduling library, and becomes the
 * "idle" thread. It returns once all the threads have finished running.
 *
 * If @preempt is `true`, then preemptive scheduling is enabled.
 *
 * Return: 0 in case of success, -1 in case of failure (e.g., memory allocation,
 * context creation).
 */
int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	current_thread = uthread_create(func, arg);

	//If it is the initial thread, create it and put it into queue.
	if (my_thread_Queue == NULL){
		my_thread_Queue = queue_create();
		if (my_thread_Queue == NULL) return -1;
		if (queue_enqueue(my_thread_Queue, current_thread) == -1) return -1;
		return 0;	
	}
	queue_enqueue(my_thread_Queue, current_thread);
	return 0;
}

void uthread_block(void)
{
	/* TODO Phase 4 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 4 */
}

