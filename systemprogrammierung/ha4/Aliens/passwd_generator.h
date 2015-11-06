/*
 * reader.h
 *
 *  Created on: 17.03.2015
 *      Author: Fubezz
 */

#ifndef PASSWD_GENERATOR_H_
#define PASSWD_GENERATOR_H_

typedef struct {
	ringbuffer* out;
	uint num_threads;
} generator_args;

void* generate_passwords_thread(void* args);

#endif /* PASSWD_GENERATOR_H_ */
