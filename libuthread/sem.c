#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	size_t count;
	struct queue *blocked_queue;
};

sem_t sem_create(size_t count)
{
	struct semaphore *sem = (struct semaphore*) malloc(sizeof(struct semaphore));
	if (sem == NULL) return NULL;

	sem->blocked_queue = queue_create();
	if (sem->blocked_queue == NULL) return NULL;

	sem->count = count;
	return sem;
}

int sem_destroy(sem_t sem)
{
	if (queue_length(sem->blocked_queue) != 0 || sem == NULL) return -1;
	queue_destroy(sem->blocked_queue);
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	if (sem == NULL) return -1;
	preempt_disable();
	// if semaphore is not available, put thread into blocked queue
	// then go to next available thread in ready queue
	while (sem->count == 0) {
		struct uthread_tcb *curr_thread;
		curr_thread = uthread_current();
		queue_enqueue(sem->blocked_queue, (void*)curr_thread);
		uthread_block();
	}
	sem->count -= 1;
	preempt_enable();
	return 0;
}

int sem_up(sem_t sem)
{
	if (sem == NULL) return -1;
	preempt_disable();
	sem->count += 1;
	// put a thread in blocked queue, if there is any, into ready queue
	if (queue_length(sem->blocked_queue) != 0) {
		struct uthread_tcb *uthread;
		queue_dequeue(sem->blocked_queue, (void**)&uthread);
		uthread_unblock(uthread);
	}
	preempt_enable();
	return 0;
}