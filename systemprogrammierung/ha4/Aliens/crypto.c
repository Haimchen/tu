#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "md5.h"

void compute_hash(const char *tohash, md5hash* hash) {
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, tohash, strlen(tohash));
	MD5_Final((unsigned char*)hash, &ctx);
}

int compare_hash(const md5hash* hash1, const md5hash* hash2 )
{
	return memcmp(hash1, hash2, sizeof(md5hash));
}

void encrypt(const byte *clear, const uint size, const byte *key, byte *chiffre) {
	uint i, j = 0;
	for (i = 0; i < size; i++) {
		//doing xor cipher for each byte in string
		chiffre[i] = clear[i] ^ key[j];
		//update key byte
		j = (j + 1) % strlen((char*)key);
	}
	fflush(stdout);
}

void decrypt(const byte *chiffre, const uint size, const byte *key, byte *clear) {
	encrypt(chiffre, size, key, clear);
}

void create_enrypted_file(char *in, char *out) {
	FILE* input = fopen(in, "r");
	if(!input) {
		perror(in);
		exit(1);
	}
	FILE* output = fopen(out, "w+");
	if(output) {
		perror(out);
		exit(1);
	}

	size_t len = 120;
	byte* line = emalloc(sizeof(unsigned char)*len);
	ssize_t read = 0;
	while ((read = getline((char**)&line, &len, input)) != -1) {
		int length = strlen((const char*)line);
		byte* lineHelp = emalloc( sizeof(byte) * length + 1);
		byte* enc = emalloc( sizeof(byte) * length + 1);
		enc[length] = 0;
		//We still need the original string
		memcpy(lineHelp,line,length+1);
		byte* passwd = (byte*) strtok((char*)lineHelp, ":");
		encrypt(line, length, passwd, enc);
		char* res = convert_bytes_to_hex(enc, length);
		strcat(res, "\r\n");
		fwrite(res, sizeof(char), strlen(res), output);
		free(lineHelp);
		free(enc);
	}
	fclose(input);
	fclose(output);

	free(line);
}
