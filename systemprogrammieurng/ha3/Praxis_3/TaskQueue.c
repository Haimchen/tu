/*
Dora Szuecs
Kevin Bock
Philipp Kueckes
Sarah Koehler
*/

#include "TaskQueue.h"

#include <stdio.h>
#include <stdlib.h>
#include "task.h"

typedef struct task_tq_elem_s
{
	int priority;
	def_task *value;
	struct task_tq_elem_s *next;
} tq_elem;

struct TaskQueue
{
	int size;
	tq_elem *root;
};

/*
Create a new, empty task queue
*/
TaskQueue* tqueue_new()
{
	// allocate memory for task queue
	TaskQueue *pq = (TaskQueue*) malloc(sizeof(TaskQueue));

	// check if task queue is created
	if (pq == NULL) {
		printf("Couldn't create Task Queue\n");
		exit(1);
	}
	pq->root = NULL;
	pq->size = 0;
	return pq;
}

/*
Delete a task queue and free all allocated memory
*/
void tqueue_free(TaskQueue *queue)
{
	// check if queue exists
	if (queue == NULL) {
		printf("Queue doesn't exist\n");
		return;
	}
	// free elements (if they exist)
	if (queue->size > 0) {
		int i;
		tq_elem* pointer;
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
insert new element in the task queue
with the given task Pointer (=value)
*/
int tqueue_offer(TaskQueue *queue, def_task *value)
{
	if (queue == NULL) {
		printf("Queue doesn't exist\n");
		return -1;
	}

	// create new element
	tq_elem* new_elem = (tq_elem*) malloc(sizeof(tq_elem));

	// check if element is created
	if (new_elem == NULL) {
		printf("Couldn't create new Queue Element\n");
		return -1;
	}

	// save value and priority in the element
	new_elem->value = value;
	new_elem->next = NULL;

	// task queue empty
	if (queue->size == 0) {
		queue->root = new_elem;
		queue->size++;
		return 1;
	}

	tq_elem* pointer = queue->root;
	while (pointer != NULL) {

		// insert at the end
		if (pointer->next == NULL) {
			pointer->next = new_elem;
				queue->size++;
				return 1;
			}
		pointer = pointer->next;
	}
	return -1;
}
/*
Return the value of the first element in the task queue
without removing it
if the queue is empty, returns -1
*/
def_task* tqueue_peek(TaskQueue *queue)
{
	if (queue == NULL) {
		printf("Queue doesn't exist\n");
		return NULL;
	}
	if (tqueue_size(queue) == 0) {
		return NULL;
	}
	return queue->root->value;
}

/*
Return the value of the first element in the task queue and remove it
if queue is empty, returns NULL
*/
def_task* tqueue_poll(TaskQueue *queue)
{
	if (queue == NULL) {
		printf("Queue doesn't exist\n");
		return NULL;
	}
	if (tqueue_size(queue) == 0) {
	return NULL;
	}

	tq_elem* pointer = queue->root;

	queue->root = queue->root->next;

	def_task* ret_val = pointer->value;
	free(pointer);
	queue->size = queue->size - 1;

	return ret_val;
}

/*
Return the number of elements of a given task queue
*/
int tqueue_size(TaskQueue *queue)
{
	if (queue == NULL) {
		printf("Queue doesn't exist\n");
		return -1;
	}
	return queue->size;
}
