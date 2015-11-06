/*
 * utils.h
 *
 *  Created on: Mar 18, 2015
 *      Author: uni
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "buffer.h"
#include <stdint.h>
#include <stdlib.h>


typedef unsigned int  uint;
typedef unsigned char uchar;
typedef uint8_t       byte;

typedef struct {
	byte raw[16];
} md5hash;

int read_hex_file(unsigned char ***lines, uint** line_length, char* filename);

unsigned char* convert_hex_to_bytes(char *string);

char* convert_bytes_to_hex(byte *bytes, uint length);

void* emalloc(size_t size);

void* erealloc(void* p, size_t size);

#endif /* UTILS_H_ */
