/*
 * PrioQueue.h
 *
 * A simple, singly linked list based priority queue.
 *
 * The values are ordered descending by their
 * priority (highest priority = head of the queue).
 *
 * Author: CIT
 */

#ifndef PRIOQUEUE_H_
#define PRIOQUEUE_H_

// Only typedef here to limit access to the members to the source file.
typedef struct PrioQueue PrioQueue;

/**
 * Creates an empty priority queue.
 *
 * returns: an empty priority queue.
 */
PrioQueue* pqueue_new();

/**
 * Deletes the queue and frees all allocated memory.
 */
void pqueue_free(PrioQueue *queue);

/*
 * Insert the value with defined priority into the queue.
 *
 * priority: the value's priority
 * value   : the value itself
 *
 * returns : the inserted value or -1 when an error occurred.
 */
int pqueue_offer(PrioQueue *queue, int priority, int value);

/**
 * Retrieves the value with the highest priority from the queue.
 *
 * returns: the value with the highest priority.
 */
int pqueue_peek(PrioQueue *queue);

/**
 * Retrieves and REMOVES the value with the highest priority from the queue.
 *
 * returns: the value with the highest priority.
 */
int pqueue_poll(PrioQueue *queue);

/**
 * The current size of the queue.
 *
 * returns: the current size of the queue.
 */
int pqueue_size(PrioQueue *queue);

/**
 * Prints the current values in the queue to stdout.
 *
 * The elements get printed in the following form:
 * 
 * (<prio>,<value>)(<prio>,<value>) ...
 *
 * with the head of the queue as first element.
 */
void pqueue_print(PrioQueue *queue);

/**
 * Iterates over all elements in the queue and passes 
 * the priority and value to the supplied function.
 *
 * func: A function expecting the priority and value as const pointers.
 */
void pqueue_apply(PrioQueue *queue, void (*func)(const int *, const int *));

#endif /* PRIOQUEUE_H_ */
