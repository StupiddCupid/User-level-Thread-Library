#define _XOPEN_SOURCE 600
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

static struct queue *Ready_Queue;
static struct queue *Exited_Queue;
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

// deallocate uthrea_tcb 
void clear_exited_queue()
{
	struct uthread_tcb *thread_to_free;
	while(queue_length(Exited_Queue) > 0) {
		queue_dequeue(Exited_Queue, (void**)&thread_to_free);
		uthread_ctx_destroy_stack(thread_to_free->context_ptr);
		free(thread_to_free);
	}
	Exited_Queue = NULL;
}

struct uthread_tcb *uthread_current(void)
{
	return current_thread;
}

void uthread_yield(void)
{
	//Make current thread "READY" and Enqueue into ready queue
	preempt_disable();
	if (current_thread == NULL){
		struct uthread_tcb *IDLE_thread;
		queue_dequeue(Ready_Queue, (void**)&IDLE_thread);
		return;
	}
	current_thread->thread_state = READY;
	queue_enqueue(Ready_Queue, (void*)current_thread);

	//Get next available thread 
	struct uthread_tcb *next_thread;
	if (queue_length(Ready_Queue) > 0) {
		queue_dequeue(Ready_Queue, (void**)&next_thread);
		struct uthread_tcb *curr_thread_ptr = current_thread;

		//Context switch 
		current_thread = next_thread;
		current_thread->thread_state = RUNNING;
		uthread_ctx_switch(curr_thread_ptr->context_ptr, 
						   next_thread->context_ptr);
	}

	preempt_enable();	
}

void uthread_exit(void)
{
	preempt_disable();

	//Make current thread "EXITED"
	current_thread->thread_state = EXITED;
	queue_enqueue(Exited_Queue, (void*)current_thread);
	struct uthread_tcb *next_thread;

	//Context switch to next available thread
	if (queue_length(Ready_Queue) > 0) {
		queue_dequeue(Ready_Queue, (void**)&next_thread);

		struct uthread_tcb *current_thread_ptr = current_thread;
		current_thread = next_thread;
		current_thread->thread_state = RUNNING;
		uthread_ctx_switch(current_thread_ptr->context_ptr, 
						   next_thread->context_ptr);
	}

	preempt_enable();
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb *new_thread = 
			(struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	new_thread->stack_ptr = uthread_ctx_alloc_stack();
	new_thread->context_ptr = 
			(uthread_ctx_t *)malloc(sizeof(uthread_ctx_t)) ;
	new_thread->thread_state = READY;

	// check if context initialized
	int retval = 0;
	retval = uthread_ctx_init(new_thread->context_ptr,
							  new_thread->stack_ptr, func, arg);
	if (new_thread->stack_ptr == NULL || retval == -1) return -1;

	preempt_disable();
	queue_enqueue(Ready_Queue, (void*)new_thread);
	preempt_enable();
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	if (Ready_Queue == NULL) Ready_Queue = queue_create();
	//create idle thread, and enqueue into Ready_Queue
	uthread_create(NULL, NULL);
	queue_dequeue(Ready_Queue, (void**)&idle_thread);
	current_thread = idle_thread;
	uthread_create(func, arg); 

	preempt_start(preempt);
	while (1){
		uthread_yield();
		if (queue_length(Ready_Queue) == 0) break;
	}
	preempt_stop();

	//free the queue
	clear_exited_queue();
	queue_destroy(Ready_Queue);
	Exited_Queue = NULL;
	Ready_Queue = NULL;
	return 0;
}

void uthread_block(void)
{
	struct uthread_tcb *next_thread;
	struct uthread_tcb *curr_thread_ptr = current_thread;

	preempt_disable();
	// set thread state and go to next available thread
	current_thread->thread_state = BLOCKED;
	queue_dequeue(Ready_Queue, (void**)&next_thread);
	current_thread = next_thread;
	uthread_ctx_switch(curr_thread_ptr->context_ptr,
					   next_thread->context_ptr);
	preempt_enable();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	uthread->thread_state = READY;
	preempt_disable();
	// put thread into ready queue
	queue_enqueue(Ready_Queue, (void*)uthread);
	preempt_enable();
}