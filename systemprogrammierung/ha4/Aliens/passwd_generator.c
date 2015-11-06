#include <stdlib.h>

#include "buffer.h"
#include "utils.h"
#include <stdio.h>
#include "string.h"
#include "passwd_generator.h"

void* generate_passwords_thread(void* args) {
	generator_args* arg_struct = (generator_args*) args;

	//TODO Create all permutations of 1 to 5 long words
	//TODO deposit in input ringbuffer

	// alphabet
	char alphabet [] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	int alphabetSize = 26;

	int i;
	for (i = 1; i <= 5; i++) {	// 5 different word sizes

		char word[i+1]; // declare word container of size i+1 (string + '\0')

		// alphabet_size**i words
		int a0;
		for (a0 = 0; a0 < alphabetSize; a0++) { // 0th character
			word[0] = alphabet[a0];
			if (i == 1) {word[i] = '\0'; deposit(arg_struct->out, word); continue;}
			int a1;
			for (a1 = 0; a1 < alphabetSize; a1++) { // 1st character
				word[1] = alphabet[a1];
				if (i == 2) {word[i] = '\0'; deposit(arg_struct->out, word); continue;}
				int a2;
				for (a2 = 0; a2 < alphabetSize; a2++) { // 2nd character
					word[2] = alphabet[a2];
					if (i == 3) {word[i] = '\0'; deposit(arg_struct->out, word); continue;}
					int a3;
					for (a3 = 0; a3 < alphabetSize; a3++) { // 3rd character
						word[3] = alphabet[a3];
						if (i == 4) {word[i] = '\0'; deposit(arg_struct->out, word); continue;}
						int a4;
						for (a4 = 0; a4 < alphabetSize; a4++) { // 4th character
							word[4] = alphabet[a4];
							word[5] = '\0'; deposit(arg_struct->out, word);
						}
					}
				}
			}
		}
	}

	// sending the last word
	deposit(arg_struct->out, NULL);

	pthread_exit(NULL);
}

