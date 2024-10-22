#define _XOPEN_SOURCE 600
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
    struct Node* new_node = (struct Node*) malloc (sizeof(struct Node));
    new_node->key = value;
    new_node->next = NULL;
    return new_node;
};

queue_t queue_create(void)
{
    struct queue* new_queue = (struct queue*) malloc(sizeof(struct queue));
    if (new_queue == NULL) { return NULL; }
    new_queue->front = NULL;
    new_queue->tail = NULL;
    new_queue->length = 0;
    return new_queue;
}

int queue_destroy(queue_t queue)
{
    if (queue == NULL || queue->length > 0) { return -1; }
    free(queue);
    return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
    //if queue is null or data is null
    if (queue == NULL || data == NULL) { return -1; }

    //create a new node, return -1 if fails to create new node
    struct Node* new_node = newNode(data);
    if (new_node == NULL) { return -1; }

    //add new node if queue is empty
    if (queue->length == 0){
        queue->front = new_node;
        queue->tail = new_node;
        queue->length += 1;
        return 0;
    }

    //add new node if len(queue) is 1
    if (queue->length == 1){
        queue->tail = new_node;
        queue->front->next = new_node;
        queue->length += 1;
        return 0;
    }

    //add new node if len(queue) > 1 
    queue->tail->next = new_node;
    queue->tail = new_node;
    queue->length += 1;
    return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
    if(queue == NULL || data == NULL || queue->length == 0) return -1;

    struct Node *curr_node;
    curr_node = queue->front;
    *data = curr_node->key;

    //if only one node left in the queue
    if (queue->length == 1) {
        queue->front = NULL;
        queue->tail = NULL;
    } else {
        queue->front = curr_node->next;
    }
    free(curr_node);
    queue->length -= 1;
    return 0;
}

int queue_delete(queue_t queue, void *data)
{
    if (queue == NULL || data == NULL) return -1;

    struct Node *prev_node = queue->front;
    struct Node *curr_node = queue->front;
    int dataFound = 0;

    while(curr_node != NULL) {
        // if current node contains the data, delete and free the node
        if (curr_node->key == data) {
            dataFound = 1;
            if(curr_node == queue->front) {
                queue->front = curr_node->next;
            }
            else if (curr_node == queue->tail) {
                queue->tail = prev_node;
            }
            else {
                prev_node->next = curr_node->next;
            }
            free(curr_node);
            queue->length -= 1;
            break;
        } 
        else {
            //if data is not found, go to the next avilable node
            if (curr_node->next == NULL) break;
            else {
                prev_node = curr_node;
                curr_node = curr_node->next;
            }
        }
    }
    curr_node = NULL;
    prev_node = NULL;
    if (dataFound == 0) return -1;
    else return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
    if(queue== NULL || func == NULL) return -1;

    struct Node *curr_node = queue->front;
    struct Node *next_node = queue->front->next;

    //loop through the queue
    while(curr_node != NULL) {
        func(queue, curr_node->key);
        if (next_node != NULL) {
            curr_node = next_node;
            next_node = next_node->next;
        }
        else break;
    }
    return 0;
}

int queue_length(queue_t queue)
{
    if (queue == NULL) return -1;
    else return queue->length;
}

