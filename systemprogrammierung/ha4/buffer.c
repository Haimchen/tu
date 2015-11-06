#include <stdio.h>
#include "buffer.h"
#include <stdlib.h>
#include "utils.h"


/*
 * A synchronized ring buffer struct with fixed size RINGBUFFER_SIZE.
 */
struct ringbuffer {
		int size;
		char * buffer[RINGBUFFER_SIZE];
		int next_read;
		int next_write;
		pthread_mutex_t mutex_next_action;
		pthread_cond_t cond_write, cond_read;
};


ringbuffer* make_buffer()
{
	int i;

	//TODO Create new Buffer
	ringbuffer *buffer = malloc(sizeof(ringbuffer));
	buffer->size = 0;
	buffer->next_read = 0;
	buffer->next_write = 0;

	for (i = 0; i < 20; i++) {
		buffer->buffer[i] = 0;
	}

	//TODO Intialize synchronization variables
	pthread_mutex_init(&(buffer->mutex_next_action), NULL);
	pthread_cond_init(&(buffer->cond_write), NULL);
	pthread_cond_init(&(buffer->cond_read), NULL);
	return buffer;
}

void deposit(ringbuffer* buf, char* str)
{
	pthread_mutex_lock(&(buf->mutex_next_action));
	while(buf->size >= 20){
	pthread_cond_wait(&(buf->cond_write), &(buf->mutex_next_action));
	}
    // TODO: SIGNAL

	//TODO Create synchronized deposition in buffer
	if (buf->buffer[buf->next_write]==0) {
		buf->buffer[buf->next_write] = str;
		buf->next_write = (buf->next_write+1)%20;
		buf->size++;
		pthread_cond_signal(&(buf->cond_read));
		pthread_mutex_unlock(&(buf->mutex_next_action));
	} else {
		printf("ungultige werte im buffer");
		exit(1);
	}
}

char* fetch(ringbuffer* buf)
{

	pthread_mutex_lock(&(buf->mutex_next_action));
		while(buf->size == 0){
		pthread_cond_wait(&(buf->cond_read), &(buf->mutex_next_action));
	}

    //TODO Create synchronized fetching from Buffer

		buf->size--;
		char * return_value = buf->buffer[buf->next_read];
		buf->buffer[buf->next_read] = 0;
		buf->next_read = (buf->next_read+1)%20;
		pthread_cond_signal(&(buf->cond_write));
		pthread_mutex_unlock(&(buf->mutex_next_action));

		return return_value;
}

void destroy_buffer(ringbuffer* buf)
{
	//TODO Free buffer content and its synchronisation variables
	pthread_mutex_destroy(&(buf->mutex_next_action));
	pthread_cond_destroy(&(buf->cond_write));
	pthread_cond_destroy(&(buf->cond_read));

	free(buf);
}

