/*
 * passwd_cracker.c
 *
 *  Created on: Mar 17, 2015
 *      Author: uni
 */

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "buffer.h"
#include "utils.h"
#include "crypto.h"
#include "passwd_cracker.h"

//check if the hash value matches a hash value from the passwords to crack
int compare_hashes(md5hash* hash, md5hash* hashes[], int num_hashes) {
	int i;
	for (i = 0; i < num_hashes; i++) {
		if (compare_hash(hash, hashes[i]) == 0) {
			return i;
		}
	}
	return -1;
}

void* password_cracker_thread(void* args) {
	cracker_args* arg_struct = (cracker_args*) args;
	
	// fetch password from input buffer

	char* passwrd = fetch(arg_struct->in);

	// compute md5 and pass it to the output thread

	md5hash* hash = NULL;

	compute_hash(passwrd, hash);

	if(compare_hashes(hash, (md5hash**)arg_struct->hashes, arg_struct->num_hashes) == 0){
		deposit(arg_struct->out, passwrd);
	}

	pthread_exit(NULL);
}
