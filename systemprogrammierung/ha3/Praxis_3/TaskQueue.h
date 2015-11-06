/*
 * TaskQueue.h
 *
 * A simple, singly linked list based queue for Tasks.
 *
 */

#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_

#include "task.h"
// Only typedef here to limit access to the members to the source file.
typedef struct TaskQueue TaskQueue;

/**
 * Creates an empty queue.
 *
 * returns: an empty  queue.
 */
TaskQueue* tqueue_new();

/**
 * Deletes the queue and frees all allocated memory.
 */
void tqueue_free(TaskQueue *queue);

/*
 * Insert the value into the queue.
 *
 * value   : the value itself (a pointer to a task)
 *
 * returns : 1 when value was inserted  or -1 when an error occurred.
 */
int tqueue_offer(TaskQueue *queue, def_task *value);

/**
 * Retrieves the first value from the queue.
 *
 * returns: the value at the head of the queue.
 */
def_task* tqueue_peek(TaskQueue *queue);

/**
 * Retrieves and REMOVES the first element from the queue.
 *
 * returns: the value at the head of the queue.
 */
def_task* tqueue_poll(TaskQueue *queue);

/**
 * The current size of the queue.
 *
 * returns: the current size of the queue.
 */
int tqueue_size(TaskQueue *queue);
#endif /* TASKQUEUE_H_ */
