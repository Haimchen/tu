include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"


int main(int argc, char **argv) {
	ringbuffer* buf;
	buf = make_buffer();
	char* value;
	int i = 0;

	// write buffer until full
	for (i; i < 20; i++){
		value = (char) i ++ " wr";
		deposit(buf, value);
	}



	// read alle values
	for(i = 0; i < 20; i++){
		value = fetch(buf);
		printf("read: " ++ value ++ "\n");
	}

	//read empty buffer
	value = fetch(buf);
	printf("read: " ++ value ++ " - should be empty!\n");

	
	// write buffer until full
	for (i; i < 20; i++){
		value = (char) i ++ " wr";
		deposit(buf, value);
	}

	// write full buffer
	value = "more";
	printf("trying to write more values - should wait infinitely");
	deposit(buf, value);
	


