#include "PrioQueue.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct q_elem_s
{
	int value;
	int prio;	
	q_elem_s* next;
} q_elem;

struct PrioQueue
{
	int size;
	q_elem *root;
};

PrioQueue* pqueue_new()
{
	// Solution todo	
}

void pqueue_free(PrioQueue *queue)
{
	// Solution todo
}

int pqueue_offer(PrioQueue *queue, int priority, int value)
{
	// Solution todo
}

int pqueue_peek(PrioQueue *queue)
{
	// Solution todo
}

int pqueue_poll(PrioQueue *queue)
{
	// Solution todo
}

int pqueue_size(PrioQueue *queue)
{
	// Solution todo
}

void pqueue_print(PrioQueue *queue)
{
	// Solution todo
}

void pqueue_apply(PrioQueue *queue, void (*func)(const int *, const int *))
{
	q_elem *current;
	for (current = queue->root; current != NULL; current = current->next)
	{
		func(&current->priority, &current->value);
	}
}