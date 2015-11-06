/*
 * utils.c
 *
 *  Created on: Mar 18, 2015
 *      Author: uni
 */

#include "utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned char* convert_hex_to_bytes(char *string) {
	if (strlen(string)%2 != 0){
		printf("Error in chat message\n");
		exit(1);
	}
	int len = strlen(string) / 2;
	unsigned char *bytes = (unsigned char*) emalloc(len);
	int i = 0;
	unsigned int u;
	for (i = 0; i < len; i++) {
		sscanf(string + 2 * i, "%02X", &u);
		bytes[i] = u;
	}
	return bytes;
}

char* convert_bytes_to_hex(byte *bytes, uint length) {
	char *string = emalloc(length * 2 + 1);
	uint i = 0;
	for (i = 0; i < length; i++) {
		sprintf(string + i * 2, "%02X", bytes[i]);
	}
	return string;
}

int read_line(char **line, FILE *file) {
	ssize_t read;
	size_t len = 120;
	read = getline(line,&len,file);
	if (read == -1) {
		return -1;
	}
	*line = strtok(*line, "\r\n");
	return strlen(*line);
}
//read a line with lenght "line_length" from a file and stores it in the "lines" array as raw bytes
int read_hex_file(unsigned char ***lines, uint** line_length, char* filename) {
	int line_num = 0;
	FILE* file;
	file = fopen(filename, "r");
	if (file != NULL) {
		char* line = NULL;
		ssize_t read;
		while ((read = read_line(&line, file)) != -1) {
			line_num++;
			unsigned char *bytes = convert_hex_to_bytes(line);
			if (line_num > 1) {
				//we dont know the length before
				*lines = realloc(*lines, line_num * sizeof(unsigned char*));
				(*lines)[line_num - 1] = bytes;
				if (line_length != NULL){
					*line_length = realloc(*line_length,line_num * sizeof(int));
					(*line_length)[line_num - 1] = strlen(line) / 2;
				}
			} else {
				(*lines)[0] = bytes;
				if (line_length != NULL)
					//two hex stand for one byte
					(*line_length)[0] = strlen(line)/2;
			}
		}
		if (line) free(line);
		fclose(file);
		return line_num;
		exit(EXIT_SUCCESS);
	} else {
		printf("Could not open file: %s\n", filename);
		exit(1);
	}
}
//function to have a save allocation of memory
void* emalloc(size_t size) {
	void *buf = malloc(size);
	if(buf == NULL){
		perror("memory allocation failed");
		exit(1);
	}
	return buf;
}
//function to have a save reallocation of memory
void* erealloc(void* p, size_t size) {
	p = realloc(p, size);
	if(p == NULL){
		perror("memory allocation failed");
		exit(1);
	}
	return p;
}
