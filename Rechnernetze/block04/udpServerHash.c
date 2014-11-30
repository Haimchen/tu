/*
############################################################################
#                                                                          #
# Copyright TU-Berlin, 2011-2014                                           #
# Die Weitergabe, Veröffentlichung etc. auch in Teilen ist nicht gestattet #
#                                                                          #
############################################################################
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_BUFFER_LENGTH 100

typedef struct list_t {
	uint16_t key;
	uint16_t value;
	struct list_t *next;
} list_t;

typedef struct hashtable_t {
	unsigned int size;
	list_t **table;
} hashtable_t;

hashtable_t *create_ht (unsigned int size) {
	hashtable_t *ht = NULL;
	unsigned int i;

	if (size < 1) return NULL;

	if ((ht = malloc(sizeof (hashtable_t))) == NULL) {
		return NULL;
	}

	if ((ht->table = malloc((sizeof (list_t *)) * size)) == NULL) {
		return NULL;
	}

	for (i = 0; i < size; i++) {
		ht->table[i] = NULL;
	}
	ht->size = size;

	return ht;
}

uint16_t hash (hashtable_t *ht, uint16_t *key) {
	unsigned char temp = (unsigned char) *key;
	return (uint16_t) temp;
}

list_t *new_pair (uint16_t *key, uint16_t *value){
	list_t *newpair;

	if((newpair = malloc(sizeof (list_t))) == NULL) {
		return NULL;
	}
	newpair->key = *key;
	newpair->value = *value;
	newpair->next = NULL;

	return newpair;
}

list_t *get_ht (hashtable_t *ht, uint16_t *key) {

	uint16_t keyPos = hash(ht, key);
	list_t *tmp = ht->table[keyPos];

	if (tmp == NULL) {
		return NULL;
	}

	if (tmp->key == *key) {
		return tmp;
	}

	for(; tmp != NULL; tmp = tmp->next) {
		if (tmp->key == *key) {
			return tmp;
		}
	}
	return NULL;
}

int set_entry (hashtable_t *ht, uint16_t *key, uint16_t *value) {

	uint16_t keyPos = hash(ht, key);
	list_t *tmp = ht->table[keyPos];    

	if (tmp == NULL) {
		ht->table[keyPos] = new_pair(key, value);
		return 0;
	}

	for(tmp = ht->table[keyPos]; tmp != NULL; tmp = tmp->next) {
		if (tmp->key == *key) {
			tmp->value = *value;
			return 0;
		}
		if (tmp->next == NULL) {
			tmp->next = new_pair(key, value);
			return 0;
		}
	}
	return -1;
}

int delete_entry (hashtable_t *ht, uint16_t *key) {
	uint16_t keyPos = hash(ht, key);
	list_t *tmp = ht->table[keyPos];
	list_t *tmp_n;

	if (tmp == NULL) {
		return -1;
	}

	if (tmp->key == *key) {
		ht->table[keyPos] = tmp->next;
		free(tmp);
		return 0;
	}

	for(; tmp != NULL; tmp = tmp->next) {
		if (tmp->next == NULL) {
			return -1;
		}
		if (tmp->next->key == *key) {
			tmp_n = tmp->next->next;
			free(tmp->next);
			tmp->next = tmp_n;
			return 0;
		}
	}
	return -1;
}

int delete_ht (hashtable_t *ht) {
	unsigned int i;
	list_t *tmp;

	for (i = 0; i < sizeof(hashtable_t); i++) {
		for(tmp = ht->table[i]; tmp != NULL; tmp = tmp->next) {
			free(tmp);
		}
	}
	free(ht);
	return 0;
}

void print_ht (hashtable_t *ht) {
	unsigned int i;
	list_t *tmp;
	printf("Key | Value");

	for (i = 0; i < ht->size; i++) {
		printf("\nKeys %d: ", i);
		if (ht->table[i] != NULL) {
			for(tmp = ht->table[i]; tmp != NULL; tmp = tmp->next) {
				printf("%d, %d | ", tmp->key, tmp->value);
			}
		}
	}

	printf("\n");
}

void unpackData(unsigned char *buffer, unsigned char *order, uint16_t *key, uint16_t *value, int* port, char *address) {
	uint16_t tmp;

	order[0] = buffer[0]; 
	order[1] = buffer[1]; 
	order[2] = buffer[2]; 
	order[3] = buffer[3]; 

	tmp = (buffer[4] << 8) | buffer[5]; 
	*key = ntohs(tmp);

	tmp = (buffer[6] << 8) | buffer[7];
	*value = ntohs(tmp);

	address[0] = buffer[8];
	address[1] = buffer[9];
	address[2] = buffer[10];
	address[3] = buffer[11];
	*port = (buffer[12] << 8) | buffer[13];
}

void packData(unsigned char *buffer, uint16_t a, uint16_t b) {
	uint16_t tmp = htons(a);

	buffer[4] = tmp >> 8; // erste Hälfte von tmp
	buffer[5] = tmp;

	tmp = htons(b);
	buffer[6] = tmp >> 8;
	buffer[7] = tmp;
}

void writeAddressToBuffer(unsigned char *buffer, struct sockaddr_in *address) {
	// all values in *address are already in network byte order (see docs)
	uint32_t ipAddress = (*address).sin_addr.s_addr;

	uint32_t *bufferPointer = (uint32_t*) &buffer[8];
	*bufferPointer = ipAddress;

	uint16_t *portPointer = (uint16_t*) &buffer[12];
	*portPointer = (*address).sin_port;
}

int main(int argc, char *argv[])
{
	int sockfd;
	int sockcl;
	struct sockaddr_in our_addr; // our address information
	struct sockaddr_in their_addr; // clients address information
	struct sockaddr_in suc_addr;
	socklen_t their_len = sizeof their_addr;
	int serverPort;
	unsigned int len;
	uint16_t key = 0;
	uint16_t value = 0;
	unsigned char order[4];
	unsigned char buffer[14];
	int port;
	char address[4];

	printf("TCP Server online\n\n");

	if (argc != 10) {
		fprintf(stderr,"Usage: tcpServer serverAdress serverPort serverID predID predAdress predport succID succAdress succPort \n");
		exit(1);
	}

	serverPort = atoi(argv[2]);
	int ourName = atoi(argv[3]);
	char *addrSucc = argv[8];
	int portSucc = atoi(argv[9]);
	int numberSucc = atoi(argv[7]);


	int ourStart = atoi(argv[4]) + 1;
	if (ourStart > ourName) {
		ourStart = 0;
	}

	/* ******************************************************************
	   TO BE DONE: Create socket
	 ******************************************************************* */

	sockfd = socket(PF_INET, SOCK_DGRAM, 0);

	//setup transport address
	our_addr.sin_family = AF_INET;
	our_addr.sin_port = htons(serverPort);
	inet_pton(AF_INET, "127.0.0.1", &(our_addr.sin_addr));
	memset(our_addr.sin_zero, '\0', sizeof our_addr.sin_zero);

	//setup successor transport address
	suc_addr.sin_family = AF_INET;
	suc_addr.sin_port = htons(portSucc);
	inet_pton(AF_INET, addrSucc, &(suc_addr.sin_addr));
	memset(suc_addr.sin_zero, '\0', sizeof suc_addr.sin_zero);

	printf("Listening on address %i at port %i\n", our_addr.sin_addr.s_addr, our_addr.sin_port);
	printf("Next server: %s at port %i\n", addrSucc, portSucc);
	printf("Accepting values from %i to %i\n", ourStart, ourName);

	len = sizeof(our_addr);
	bind(sockfd,(struct sockaddr *) &our_addr, len);

	hashtable_t *ht = create_ht(256); 

	list_t *tmp;

	printf("Listening for Client\n");

	while(1) {
		memset(buffer, 0, sizeof buffer);
		port = 0;
		memset(address, 0, sizeof address);

		recvfrom(sockfd, buffer, sizeof (buffer), 0, (struct sockaddr *) &their_addr, &their_len);
		printf("Data received: %s\n", buffer);  
		unpackData(buffer, order, &key, &value, &port, address);
		printf("key: %i \n", key);
		printf("value: %i \n", value);
		uint16_t hashValue = hash(ht, &key);
		printf("hashValue: %u \n", hashValue);

		// Anfrage von Client
		if (port == 0) {

			// wir sind zustandig
			if (hashValue <= ourName && hashValue >= ourStart) {
				printf("Request from Client - my job\n");
			} else { // weiterleiten
				printf("Request from Client - not my job\n");
				writeAddressToBuffer(buffer, &our_addr);
				sendto(sockfd, buffer, sizeof buffer, 0, (struct sockaddr *) &suc_addr, sizeof suc_addr);
				printf("Request from Client - sent to next server\n");
				recvfrom(sockfd, buffer, sizeof buffer, 0, NULL, NULL);
				printf("Received reply from server %i, port %i\n", *((uint32_t*)(&buffer[8])), *((uint16_t*)(&buffer[12])));
				sendto(sockfd, buffer, sizeof buffer, 0, (struct sockaddr *) &their_addr, sizeof their_addr);
				continue;
			}
		} else {
			// weitergeleitet von Server
			if (hashValue <= ourName && hashValue >= ourStart) {
				// wir sind zustandig
				printf("Request from other server - my job\n");
				writeAddressToBuffer(buffer, &our_addr);
			} else { 
				// weiterleiten
				printf("Request from other server - sent to next server\n");
				sendto(sockfd, buffer, sizeof buffer, 0, (struct sockaddr *) &suc_addr, sizeof suc_addr);
				continue;
			}
		}

		if (strncmp("GET", order, 3) == 0) {
			tmp = get_ht(ht, &key);
			if (tmp == NULL) {
				strcpy(buffer, "NOF");
			} else {
				strcpy(buffer, "VAL ");
				packData(buffer, tmp->key, tmp->value);
			}           
		} else if (strncmp("SET", order, 3) == 0) {
			int tmp = set_entry(ht, &key, &value);
			if (tmp < 0) {
				strcpy(buffer, "ERR");
			} else {
				strcpy(buffer, "OK!");
			}   
		} else if (strncmp("DEL", order, 3) == 0) {
			int tmp = delete_entry(ht, &key);
			if (tmp < 0) {
				strcpy(buffer, "NOF");
			} else {
				strcpy(buffer, "OK!");
			}
		} else {
			printf("Order not found\n");
			strcpy(buffer, "ERR");
		}

		// send prepared answer
		sendto(sockfd, buffer, sizeof buffer, 0, (struct sockaddr *) &their_addr, their_len);
	}

	close(sockfd);

	return 0;
}
