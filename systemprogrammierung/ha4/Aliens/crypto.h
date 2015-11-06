
#ifndef CRYPTO_H_
#define CRYPTO_H_

#include "utils.h"

void compute_hash(const char* tohash, md5hash* hash);
int  compare_hash(const md5hash* hash1, const md5hash* hash2 );

void encrypt(const byte *clear,   const uint size, const byte *key, byte *chiffre);
void decrypt(const byte *chiffre, const uint size, const byte *key, byte *clear);

void create_enrypted_file(char *in, char *out);

#endif /* CRYPTO_H_ */
