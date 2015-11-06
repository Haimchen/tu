/*
 * cipher.c
 *
 *  Created on: Mar 17, 2015
 *      Author: uni
 */

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "utils.h"
#include "crypto.h"
#include "passwd_decrypter.h"

#define FILENAME "communications/solution.txt" 

FILE* solution;
 
static void print_solutions(FILE* solution, decrypter_args* arg_struct, byte* passwd);

void* decrypter_messages_thread(void* args) {
	decrypter_args* arg_struct = (decrypter_args*) args;

	// Create solution.txt file
	solution = fopen("solution.txt","w");
	if(!solution){
		perror("ERROR");
		exit(1);
	}

	// fetch password from output buffer
	char* passwrd = fetch(arg_struct->in);


	// Handling if all crackerthreads are fishished

	if(passwrd == NULL){
		printf("got NULL as endcode.finishing.\n");
		pthread_exit(NULL);
	}

	// Use print_solutions to fill file

	print_solutions(solution, arg_struct, passwrd);

	pthread_exit(NULL);
}


static void print_solutions(FILE* solution, decrypter_args* arg_struct, byte* passwd)
{
	uint i;
	fprintf(solution, "\n------------Key: %s-------------\n", passwd);
	for (i = 0; i < arg_struct->num_lines; i++) {
		//get a hexcoded line
		byte* line = (byte*) arg_struct->chat_lines[i];
		//get its length
		uint length = arg_struct->line_length[i];
		//create buffer for decryption, +1 for 0 Byte
		byte* dec = (byte*) emalloc( sizeof(byte) * (length+1));
		//add "\0" byte
		dec[length] = 0;
		//decipher the line
		decrypt( line, length, passwd, dec);

		//print to solution.txt
		fprintf(solution,"\n%s\n", dec);
		free(dec);
	}
}
