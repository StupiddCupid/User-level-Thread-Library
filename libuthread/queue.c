#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct queue 
{
	int length;
    struct Node *front, *tail;
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

/*
 * queue_enqueue - Enqueue data item
 * @queue: Queue in which to enqueue item
 * @data: Address of data item to enqueue
 *
 * Enqueue the address contained in @data in the queue @queue.
 *
 * Return: -1 if @queue or @data are NULL, or in case of memory allocation error
 * when enqueing. 0 if @data was successfully enqueued in @queue.
 */
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

/*
 * queue_dequeue - Dequeue data item
 * @queue: Queue in which to dequeue item
 * @data: Address of data pointer where item is received
 *
 * Remove the oldest item of queue @queue and assign this item (the value of a
 * pointer) to @data.
 *
 * Return: -1 if @queue or @data are NULL, or if the queue is empty. 0 if @data
 * was set with the oldest item available in @queue.
 */
int queue_dequeue(queue_t queue, void **data)
{
    if(queue == NULL || data == NULL || queue->length == 0) return -1;
    struct Node *frontNode = queue->front;
    queue->front = queue->front->next;
    data = frontNode->key;
    return 0;
}

/*
 * queue_delete - Delete data item
 * @queue: Queue in which to delete item
 * @data: Data to delete
 *
 * Find in queue @queue, the first (ie oldest) item equal to @data and delete
 * this item.
 *
 * Return: -1 if @queue or @data are NULL, of if @data was not found in the
 * queue. 0 if @data was found and deleted from @queue.
 */
int queue_delete(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL) return -1;
    struct Node *prevNode = queue->front, *currNode = queue->front;
    int dataFound = 0;
    for(int i = 0; i < queue->length; i++) {
        if (currNode->key == data) {
            dataFound = 1;
            prevNode->next = currNode->next;
            free(currNode);
            break;
        }
    }
    if (dataFound == 0) return -1;
    else return 0;
}

/*
 * queue_iterate - Iterate through a queue
 * @queue: Queue to iterate through
 * @func: Function to call on each queue item
 *
 * This function iterates through the items in the queue @queue, from the oldest
 * item to the newest item, and calls the given callback function @func on each
 * item. The callback function receives the current data item as parameter.
 *
 * Note that this function should be resistant to data items being deleted
 * as part of the iteration (ie in @func).
 *
 * Return: -1 if @queue or @func are NULL, 0 otherwise.
 */
int queue_iterate(queue_t queue, queue_func_t func)
{
	if(queue== NULL || func == NULL) return -1;
    for(int i = 0; i < queue->length; i++) {

    }

    return 0;
}

int queue_length(queue_t queue)
{
    if (queue == NULL) return -1;
    else return queue->length;
}
