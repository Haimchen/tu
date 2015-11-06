/*
 * passwd_crcker.h
 *
 *  Created on: Mar 17, 2015
 *      Author: uni
 */

typedef struct {
	ringbuffer* in;
	ringbuffer* out;
	unsigned char **hashes;
	uint num_hashes;
} cracker_args;

void* password_cracker_thread(void* args);
