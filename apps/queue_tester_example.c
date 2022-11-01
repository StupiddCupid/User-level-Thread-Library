#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

void test_delete()
{
    	printf("testing delete\n");

    	static int d1 = 1;
    	static int d2 = 2;
    	static int d3 = 3;
    	static int d4 = 4;
    	static int d5 = 5;
	queue_t q;

	q = queue_create();
    	printf("test delete\n\n");
    	queue_enqueue(q, &d1);
    	queue_enqueue(q, &d2);
    	queue_enqueue(q, &d3);
    	queue_enqueue(q, &d4);
    	queue_enqueue(q, &d5);
    	queue_delete(q, &d4);
    	TEST_ASSERT(queue_length(q) == 4);
    	printf("finished testing delete\n");
}

static void iterator_inc(queue_t q, void *data)
{
    	// printf("in process of iteration\n");
    	int *a = (int*)data;
    
    	if (*a == 42){
        	//printf("there is a 42 in queue, deleting it\n");
        	queue_delete(q, data);
        	printf("deletion done\n");
    	}
    	else
        	*a += 1;
}

void test_iterator(void)
{
    	queue_t q;
    	int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    	size_t i;
    	/* Initialize the queue and enqueue items */
    	q = queue_create();
    	//printf("queue created\n");
    	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        	queue_enqueue(q, &data[i]);

    	/* Increment every item of the queue, delete item '42' */
    	printf("testing iteration function\n");
    	queue_iterate(q, iterator_inc);
    	printf("finished iteration\n");
    	TEST_ASSERT(data[0] == 2);
    	TEST_ASSERT(queue_length(q) == 9);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int *ptr;

    	static int d1 = 1;
    	static int d2 = 2;
    	static int d3 = 3;
    	static int d4 = 4;
    	static int d5 = 5;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");
	q = queue_create();
    	printf("Hiiii\n");
    	queue_enqueue(q, &d1);
    	queue_enqueue(q, &d2);
    	queue_enqueue(q, &d3);
    	queue_enqueue(q, &d4);
    	queue_enqueue(q, &d5);
    	//fprintf(stderr, "the first element in the queue is %d and the last is %d \n", *(q.first->content),*(q.last->content));
    	queue_dequeue(q, (void**)&ptr);
    	TEST_ASSERT(ptr == &d1);
    	queue_dequeue(q, (void**)&ptr);
    	TEST_ASSERT(ptr == &d2);
    	queue_dequeue(q, (void**)&ptr);
    	TEST_ASSERT(ptr == &d3);
    	queue_dequeue(q, (void**)&ptr);
    	TEST_ASSERT(ptr == &d4);
    	queue_dequeue(q, (void**)&ptr);
    	TEST_ASSERT(ptr == &d5);
    	queue_destroy(q);
}

int main(void)
{
	test_create();
	test_queue_simple();
    	test_iterator();
    	test_delete();
	return 0;
}

