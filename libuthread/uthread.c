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
queue_t ready_queue;
static struct uthread_tcb *current_thread;
static struct uthread_tcb *idle_thread;

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
	//Make current thread "READY" and Enqueue it into queue
	current_thread->thread_state = READY;
	queue_enqueue(ready_queue, (void**)&current_thread);

	//Get the head of the queue
	struct uthread_tcb *next_thread;
	queue_dequeue(ready_queue, (void**)&next_thread);

	//Context switch 
	uthread_ctx_switch(current_thread->context_ptr, next_thread->context_ptr);
	current_thread = next_thread;
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
	/* 
		called when queue is empty
	   	context switch(current_thread, idle_thread)
	*/

	//Make current thread "EXITED"
	current_thread->thread_state = EXITED;
	//Get the head of the queue to be the current thread
	uthread_ctx_switch(current_thread->context_ptr, idle_thread->context_ptr);
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
	printf("in uthread create\n");
	struct uthread_tcb *myThread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	myThread -> stack_ptr = uthread_ctx_alloc_stack();
	int retval = uthread_ctx_init(myThread->context_ptr, myThread->stack_ptr, func, arg);
	
	if (myThread->stack_ptr == NULL || retval == -1) return -1;
	myThread -> thread_state = READY;
	if(queue_enqueue(ready_queue, (void**)&current_thread) == -1) return -1;
	return 0;
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
	printf("in uthread run\n");
	if (preempt) printf("preemptive enable\n");
	ready_queue = queue_create();
	//clear the idle thread, background thread
	
	int retval = uthread_create(func, arg);
	if (retval == -1) return -1;
	current_thread = idle_thread;

	printf("in while loop\n");
	while (1){
		uthread_yield();
		if (queue_length(ready_queue) == 0) {
			uthread_exit();
			break;
		}
	}

	//free the queue
	free(ready_queue);
	return 0;
}

// void uthread_block(void)
// {
// 	/* TODO Phase 4 */
// }

// void uthread_unblock(struct uthread_tcb *uthread)
// {
// 	/* TODO Phase 4 */
// }


