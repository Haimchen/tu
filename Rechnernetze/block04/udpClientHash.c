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
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>

#define MAX_BUFFER_LENGTH 100

int packData(char *buffer, char *order, unsigned int key, unsigned int value) {
    snprintf(buffer, sizeof order, "%s", order);
    buffer[4] = key >> 8;
    buffer[5] = key;
    buffer[6] = value >> 8;
    buffer[7] = value;

    return 0;
}

void unpack(unsigned char *buffer, unsigned int *key, unsigned int *value) {
    *key = (buffer[4] << 8) | buffer[5];
    *value = (buffer[6] << 8) | buffer[7];
}

int getServer(unsigned int key, struct sockaddr *their_addr, int their_size, int sockfd) {
	char order[8];
	packData(order, "GET", key, 0);

	sendto(sockfd, order, sizeof(order), 0, their_addr, their_size);

	char buffer[8];

	recv(sockfd, buffer, sizeof buffer, 0);

	if (buffer[0] == 'N' && buffer[1] == 'O' && buffer[2] == 'F') {
		printf("Key %i not found\n", key);
		return -1;
	} else {
		unsigned int key_in = 0;
		unsigned int value_in = 0;

		unpack(buffer, &key_in, &value_in);

		printf("Returned: key = %i | value = %i\n", key_in, value_in);
		return 0;
	}
}

int setServer(unsigned int key, unsigned int value, struct sockaddr *their_addr, int their_size, int sockfd) {
	char order[8];
	packData(order, "SET", key, value);

	sendto(sockfd, order, sizeof(order), 0, their_addr, their_size);

	char buffer[8];

	recv(sockfd, buffer, sizeof buffer, 0);

	if (buffer[0] == 'O' && buffer[1] == 'K' && buffer[2] == '!') {
		//printf("New in table: key = %d | value = %d\n", key, value);
		printf("Added to table\n");
		return 0;
	} else {
		printf("An error happens! Value not added.\n");
		return -1;
	}
}

int delServer(unsigned int key, struct sockaddr *their_addr, int their_size, int sockfd) {
	char order[8];
	packData(order, "DEL", key, 0);

	sendto(sockfd, order, sizeof(order), 0, their_addr, their_size);

	char buffer[8];

	recv(sockfd, buffer, sizeof buffer, 0);

	if (buffer[0] == 'O' && buffer[1] == 'K' && buffer[2] == '!') {
		printf("Removed key %i\n", key);
		return 0;
	} else {
		printf("An error happens! Value not removed.\n");
		return -1;
	}
}


int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    int serverPort;
	unsigned int i;
	unsigned int n = 4;
	unsigned int keys[n];
    unsigned int a = 0;
    int b = 0;
    int c = 0;


    printf("TCP client example\n\n");

    if (argc != 3) {
        fprintf(stderr,"Usage: tcpClient serverName serverPort\n");
        exit(1);
    }

    serverPort = atoi(argv[2]);

    //Resolv hostname to IP Address
    if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info
        herror("gethostbyname");
        exit(1);
    }
    /* ******************************************************************
    Create Socket
    ******************************************************************* */

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    
    //setup transport address
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(serverPort);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

    
    /* ******************************************************************
    Send Data
    ******************************************************************* */


	srand(time(NULL));

	for (i = 0; i < n; i++) {
		keys[i] = rand() % 65536;
	}

	for (i = 0; i < n; i++) {
		a = rand() % 65536;
    printf("key: %i \n", (unsigned int)keys[i]);
    printf("value: %i \n", a);
    setServer(keys[i], a, (struct sockaddr *) &their_addr, sizeof their_addr , sockfd);
	}

	for (i = 0; i < n; i++) {
		getServer(keys[i], (struct sockaddr *) &their_addr, sizeof their_addr , sockfd);
	}

	for (i = 0; i < n; i++) {
		delServer(keys[i], (struct sockaddr *) &their_addr, sizeof their_addr , sockfd);
	}


	for (i = 0; i < n; i++) {
		getServer(keys[i], (struct sockaddr *) &their_addr, sizeof their_addr , sockfd);
	}

    
    /* ******************************************************************
     Close socket
    ******************************************************************* */
    close(sockfd);

    return 0;
}
