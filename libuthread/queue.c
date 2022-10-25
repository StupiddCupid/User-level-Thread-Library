#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>//delete when submit
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
    if (myQueue == NULL){
        return NULL;
    }
    myQueue->length = 0;
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
    if (queue == NULL || data == NULL){
        return -1;
    }

    //create a new node, return -1 if fails to create new node
    struct Node* new_Node = newNode(data);

    if (new_Node == NULL){
        return -1;
    }

    //add new node if queue is empty
    if (queue->length == 0){
        printf("The key in queue is %d\n", *(int *)data);
        queue->front = new_Node;
        struct Node* tail_Node = newNode(data);
        queue->tail = tail_Node;
        queue->length += 1;
        printf("The key in front is %d\n", *(int *)queue->front->key);
        printf("The key in tail is %d\n", *(int *)queue->tail->key);
        return 0;

    }
    if (queue->length == 1){
        queue->tail->next = new_Node;
        queue->tail = queue->tail->next;
        queue->front->next = queue->tail;
        queue->length += 1;
        return 0;
    }
    //add new node if queue is !empty
    queue->tail->next = new_Node;
    queue->tail = queue->tail->next;
    queue->length += 1;
    // printf("The new node in tail is %d\n", *(int *)queue->tail->key);
    // printf("The new node in front is %d\n", *(int *)queue->front->key);
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
    printf("The key in tail is %d\n", *(int *)queue->tail->key);
    if(queue == NULL || data == NULL || queue->length == 0) return -1;

    //grap the node    
    if(queue->length == 1) {
        free(queue->front);
        free(queue->tail);
        return 0;
    }
    
    // printf("New front in front is %d\n", *(int *)queue->front->key);
    *data = queue->front->key;
    struct Node *frontNode = queue->front;
    queue->front = queue->front->next;
    free(frontNode);
    queue->length -= 1;
    
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
    while(currNode != NULL) {
        // if current node contains the data, delete and free the node
        if (currNode->key == data) {
            dataFound = 1;
            if(currNode == queue->front) {
                queue->front = currNode->next;
            }
            else if (currNode == queue->tail) {
                queue->tail = prevNode;
            }
            else {
                prevNode->next = currNode->next;
            }
            free(currNode);
            break;
        }
        prevNode = currNode;
        currNode = currNode->next;
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
    struct Node *currNode = queue->front;
    while(currNode != NULL) {
        func(queue, currNode->key);
        currNode = currNode->next;
    }

    return 0;
}

int queue_length(queue_t queue)
{
    if (queue == NULL) return -1;
    else return queue->length;
}
