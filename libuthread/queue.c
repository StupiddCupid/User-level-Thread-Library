#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct queue 
{
	int length;
    struct Node*front, *tail;
};

struct Node
{
    void *key;
    struct Node* next;
};

struct Node* newNode(void* value)
{
    struct Node* newnode = (struct Node*) malloc (sizeof(struct Node));
    newnode->key = value;
    newnode->next = NULL;
    return newnode;
};

queue_t queue_create(void)
{
    struct queue* myQueue = (struct queue*) malloc(sizeof(struct queue));
    myQueue->front = NULL;
    myQueue->tail = NULL;
    if (myQueue == NULL){
        return NULL;
    }
    return myQueue;
}

int queue_destroy(queue_t queue)
{
	if (queue == NULL || queue->length > 0){
        return -1;
    }
    free(queue);
    return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
    //if queue is null or data is null
    if (queue->tail == NULL || data == NULL){
        return -1;
    }

    //create a new node, return -1 if fail to create new node
    struct Node* new_Node = newNode(data);
    if (new_Node == NULL){
        return -1;
    }

    //add new node to the end of the queue and change the tail
    queue->tail->next = new_Node;
    queue->tail = new_Node;
    return 0;
	
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}
