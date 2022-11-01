#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	size_t count;
	struct queue *blocked_queue;
	/* TODO Phase 3 */
};

/*
 * sem_create - Create semaphore
 * @count: Semaphore count
 *
 * Allocate and initialize a semaphore of internal count @count.
 *
 * Return: Pointer to initialized semaphore. NULL in case of failure when
 * allocating the new semaphore.
 */
sem_t sem_create(size_t count)
{
	struct semaphore *sem = (struct semaphore*) malloc(sizeof(struct semaphore));
	if (sem == NULL) return NULL;
	sem->blocked_queue = queue_create();
	if (sem->blocked_queue == NULL) return NULL;
	sem->count = count;
	return sem;
}

/*
 * sem_destroy - Deallocate a semaphore
 * @sem: Semaphore to deallocate
 *
 * Deallocate semaphore @sem.
 *
 * Return: -1 if @sem is NULL or if other threads are still being blocked on
 * @sem. 0 is @sem was successfully destroyed.
 */
int sem_destroy(sem_t sem)
{
	if (queue_length(sem->blocked_queue) != 0 || sem == NULL) return -1;
	queue_destroy(sem->blocked_queue);
	free(sem);
	return 0;
	
}

/*
 * sem_down - Take a semaphore
 * @sem: Semaphore to take
 *
 * Take a resource from semaphore @sem.
 *
 * Taking an unavailable semaphore will cause the caller thread to be blocked
 * until the semaphore becomes available.
 *
 * Return: -1 if @sem is NULL. 0 if semaphore was successfully taken.
 */
int sem_down(sem_t sem)
{
	// how to break out while loop
	while (sem->count == 0) {
		struct uthread_tcb *curr_thread;
		curr_thread = uthread_current();
		queue_enqueue(sem->blocked_queue, (void*)curr_thread);
		uthread_block();
	}
	sem->count -= 1;
	return 0;
}

/*
 * sem_up - Release a semaphore
 * @sem: Semaphore to release
 *
 * Release a resource to semaphore @sem.
 *
 * If the waiting list associated to @sem is not empty, releasing a resource
 * also causes the first thread (i.e. the oldest) in the waiting list to be
 * unblocked.
 *
 * Return: -1 if @sem is NULL. 0 if semaphore was successfully released.
 */
int sem_up(sem_t sem)
{
	if (sem == NULL) return -1;
	sem->count += 1;
	if (queue_length(sem->blocked_queue) != 0) {
		struct uthread_tcb *uthread;
		queue_dequeue(sem->blocked_queue, (void**)&uthread);
		uthread_unblock(uthread);
	}
	return 0;
}

