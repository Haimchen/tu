#ifndef BUFFER_H_
#define BUFFER_H_

#include <pthread.h>

/*
 * The size of the ring buffer.
 */
#define RINGBUFFER_SIZE 20

/*
 * A synchronized ring buffer struct with fixed size RINGBUFFER_SIZE.
 */
typedef struct ringbuffer ringbuffer;

/*
 * Allocates and initializes a new ring buffer.
 */
ringbuffer* make_buffer(void);

/*
 * Insert the specified char sequence into the ring buffer.
 * 
 * If the buffer is full, the function blocks until the char sequence
 * can be inserted (an char sequence is fetched).
 *
 * buf: the ring buffer
 * str: the char sequence
 */
void deposit(ringbuffer* buf, char* str);

/*
 * Returns and removes the first element (char sequence) of the specified ring buffer.
 *
 * If the buffer is empty, the functions blocks until an element is available.
 *
 * buf: the ring buffer
 *
 * returns: the first element (char sequence) of the ring buffer.
 */
char* fetch(ringbuffer* buf);

/*
 * Frees the specified ring buffer.
 */
void destroy_buffer(ringbuffer* buf);

#endif /* BUFFER_H_ */
