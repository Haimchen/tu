/*
 * test.c
 *
 *  Created on: Mar 17, 2015
 *      Author: uni
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "passwd_cracker.h"
#include "passwd_decrypter.h"
#include "passwd_generator.h"
#include "utils.h"
#include "crypto.h"

int main(int argc, char **argv) {
	if (argc > 1){
		if (strcmp(argv[1],"enc") == 0){
			create_enrypted_file("communications/chat.txt", "communications/chat_enc.txt");
			return 0;
		}
		fprintf(stderr, "usage: %s [enc]\n", argv[0]);
		return 1;
	}
	unsigned char **hashes = emalloc(sizeof(unsigned char*));
	unsigned char **chat_lines = emalloc(sizeof(unsigned char*));
	uint *line_length =  emalloc(sizeof(int));
	//Load Files
	int hash_num = read_hex_file(&hashes,NULL, "textFiles/hashs.txt");
	int line_num = read_hex_file(&chat_lines, &line_length, "communications/chat_enc.txt");

	printf("Read hashes: %i\n", hash_num);
	printf("Read: lines: %i\n", line_num);
	//Count of Cracker Threads
	int num_cracker_threads = 2;
	//declare thread names
	pthread_t generator, cracker[num_cracker_threads], decipher;
	//declare ringbuffers
	ringbuffer *in, *out;
	//initialize
	in = make_buffer();
	out = make_buffer();

	//create arguments for reader threads
	generator_args* gen_args = (generator_args*) emalloc(sizeof(generator_args));
	gen_args->out = in;
	gen_args->num_threads = num_cracker_threads;

	//create arguments for decrypter thread
	decrypter_args* dec_args = (decrypter_args*) emalloc(sizeof(decrypter_args));
	dec_args->in = out;
	dec_args->chat_lines = chat_lines;
	dec_args->line_length = line_length;
	dec_args->num_lines = line_num;
	dec_args->num_threads = num_cracker_threads;

	//create arguments for cracker threads
	cracker_args* crack_args = (cracker_args*) emalloc(sizeof(cracker_args));
	crack_args->hashes = hashes;
	crack_args->in = in;
	crack_args->out = out;
	crack_args->num_hashes = hash_num;

	int result = 0;
	//start generator thread
	result |= pthread_create(&generator, NULL, generate_passwords_thread,
			(void*) gen_args);
	int i;
	//start cracker threads
	for (i = 0; i < num_cracker_threads; ++i) {
		result |= pthread_create(&cracker[i], NULL, password_cracker_thread,
				(void*) crack_args);
	}
	//start decrypter thread
	result |= pthread_create(&decipher, NULL, decrypter_messages_thread,
			(void*) dec_args);

	//wait for join of threads
	pthread_join(generator, NULL);
	for (i = 0; i < num_cracker_threads; ++i) {
		pthread_join(cracker[i], NULL);
	}
	pthread_join(decipher, NULL);

	//free memory
	for (i = 0;i<hash_num;i++){
		free(hashes[i]);
	}
	free(hashes);
	for (i = 0; i < line_num;i++){
		free(chat_lines[i]);
	}
	free(chat_lines);
	free(line_length);
	free(dec_args);
	free(crack_args);

	return 0;
}
