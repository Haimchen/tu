/*
Dora Szuecs
Kevin Bock
Philipp Kueckes
Sarah Koehler
*/

#include "PrioQueue.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct q_elem_s
{
	int priority;
	int value;
	struct q_elem_s *next;
} q_elem;

struct PrioQueue
{
	int size;
	q_elem *root;
};

/*
Create a new, empty priority queue
*/
PrioQueue* pqueue_new()
{
	// allocate memory for priority queue
	PrioQueue *pq = (PrioQueue*) malloc(sizeof(PrioQueue));
	
	// check if priority queue is created	
	if (pq == NULL) {
		printf("Couldn't create Priority Queue\n");
		exit(1);
	}
	pq->root = NULL;
	pq->size = 0;
	return pq;
}

/*
Delete a priority queue and free all allocated memory
*/
void pqueue_free(PrioQueue *queue)
{	
	// check if queue exists
	if (queue == NULL) {
		printf("Queue doesn't exist\n");
		return;
	}
	// free elements (if they exist)
	if (queue->size > 0) {
		int i;
		q_elem* pointer;
		for (i = 1; i <= queue->size; i++) {
			pointer = queue->root;
			queue->root = queue->root->next;
			free(pointer);		
		}
	}
	// free queue after all elements are gone
	free(queue);
}

/*
insert new element in the priority queue
with the given value and priority
*/
int pqueue_offer(PrioQueue *queue, int priority, int value)
{
	if (queue == NULL) {
		printf("Queue doesn't exist\n");
		return -1;
	}

	// create new element 
	q_elem* new_elem = (q_elem*) malloc(sizeof(q_elem));

	// check if element is created
	if (new_elem == NULL) {
		printf("Couldn't create new Queue Element\n");
		return -1;
	}

	// save value and priority in the element
	new_elem->priority = priority;
	new_elem->value = value;
	new_elem->next = NULL;

	// priority queue empty
	if (queue->size == 0) {
		queue->root = new_elem;
		queue->size++;
		return value;
	}

	// insert at front
	if (queue->root->priority < priority) {		
		new_elem->next = queue->root;
		queue->root = new_elem;
		queue->size++;
		return value;
	}

	q_elem* pointer = queue->root;
	while (pointer->priority >= priority) {
			
		// insert at the end
		if (pointer->next == NULL) {
			pointer->next = new_elem;
				queue->size++;
				return value;
			}
		// place found between two elements 
		if (pointer->next->priority < priority) {
			new_elem->next = pointer->next;
			pointer->next = new_elem;
			queue->size++;
			return value;
		}
		pointer = pointer->next;
	}
	return -1;
}
/*
Return the value of the first element in the priority queue
without removing it
*/
int pqueue_peek(PrioQueue *queue)
{
	if (queue == NULL) {
		printf("Queue doesn't exist\n");
		return -1;
	}
	if (pqueue_size(queue) == 0) {
		printf("Queue is empty\n");
		return -1;
	}
	return queue->root->value;
}

/*
Return the value of the first element in the priority queue and remove it
*/
int pqueue_poll(PrioQueue *queue)
{
	if (queue == NULL) {
		printf("Queue doesn't exist\n");
		return -1;
	}
	if (pqueue_size(queue) == 0) {
	printf("Queue is empty\n");
	return -1;
	}

	q_elem* pointer = queue->root;
	
	queue->root = queue->root->next;

	int ret_val = pointer->value; 
	free(pointer);
	queue->size = queue->size - 1;
	
	return ret_val;
}

/*
Return the number of elements of a given priority queue
*/
int pqueue_size(PrioQueue *queue)
{
	if (queue == NULL) {
		printf("Queue doesn't exist\n");
		return -1;
	}
	return queue->size;
}

/*
Print the contents of the priority queue, starting with the highest priority
*/
void pqueue_print(PrioQueue *queue)
{
	if (queue == NULL) {
		printf("Queue doesn't exist\n");
		return;
	}
	if (queue->size == 0) {
		printf("priority queue is empty\n");
	} else {
		q_elem* pointer;
		for (pointer = queue->root; pointer != NULL; pointer = pointer->next) {
			printf("(%d,%d) - ", pointer->priority, pointer->value);
		}
	}
	printf("NULL\n");
}

void pqueue_apply(PrioQueue *queue, void (*func)(const int *, const int *))
{
	q_elem *current;
	for (current = queue->root; current != NULL; current = current->next)
	{
		func(&current->priority, &current->value);
	}
}
