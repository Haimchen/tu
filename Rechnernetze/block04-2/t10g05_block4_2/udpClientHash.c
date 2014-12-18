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

int packData(char *buffer, char *order, uint16_t key, uint16_t value) {
	strcpy(buffer, order);

	uint16_t tmp = htons(key);
	buffer[4] = tmp >> 8;
	buffer[5] = tmp;

	tmp = htons(value);
	buffer[6] = tmp >> 8;
	buffer[7] = tmp;

	return 0;
}

void unpack(unsigned char *buffer, uint16_t *key, uint16_t *value) {
	uint16_t tmp;

	tmp = (buffer[4] << 8) | buffer[5];
	*key = ntohs(tmp);

	tmp = (buffer[6] << 8) | buffer[7];
	*value = ntohs(tmp);
}

int getServer(uint16_t key, struct sockaddr *their_addr, int their_size, int sockfd) {
	unsigned char order[8];
	packData(order, "GET", key, 0);

	sendto(sockfd, order, sizeof(order), 0, their_addr, their_size);

	unsigned char buffer[8];

	recv(sockfd, buffer, sizeof buffer, 0);

	if (buffer[0] == 'N' && buffer[1] == 'O' && buffer[2] == 'F') {
		printf("Key %i not found\n", key);
		return -1;
	} else {
		uint16_t key_in = 0;
		uint16_t value_in = 0;

		unpack(buffer, &key_in, &value_in);

		printf("Returned: key = %i | value = %i\n", key_in, value_in);
		return 0;
	}
}

int setServer(uint16_t key, uint16_t value, struct sockaddr *their_addr, int their_size, int sockfd) {
	unsigned char order[8];
	packData(order, "SET", key, value);

	sendto(sockfd, order, sizeof(order), 0, their_addr, their_size);

	unsigned char buffer[8];

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

int delServer(uint16_t key, struct sockaddr *their_addr, int their_size, int sockfd) {
	unsigned char order[8];
	packData(order, "DEL", key, 0);

	sendto(sockfd, order, sizeof(order), 0, their_addr, their_size);

	unsigned char buffer[8];

	recv(sockfd, buffer, sizeof buffer, 0);

	if (buffer[0] == 'O' && buffer[1] == 'K' && buffer[2] == '!') {
		printf("Removed key %i\n", key);
		return 0;
	} else {
		printf("An error happens! Value not removed.\n");
		return -1;
	}
}



void calulate_duration(long *duration_pointer, struct timespec time_before, struct timespec time_after) {
		long time_taken = (time_after.tv_sec - time_before.tv_sec) * ((long) pow(10, 9));
		time_taken = time_taken + (time_after.tv_nsec - time_before.tv_nsec);

		*duration_pointer = time_taken;
}

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in their_addr; // connector's address information
	struct hostent *he;
	int serverPort;
	uint16_t i;
	unsigned int n = 250;
	uint16_t keys[n];
	uint16_t value = 0;

	struct timespec time_before;
	struct timespec time_after;
	long  durations[4 * n];
	long *duration_pointer = &durations[0];


	printf("TCP client example\n\n");

	if (argc != 3) {
		fprintf(stderr,"Usage: tcpClient serverName serverPort\n");
		exit(1);
	}

	serverPort = atoi(argv[2]);

	// Resolv hostname to IP Address
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
		keys[i] = rand();
	}

	for (i = 0; i < n; i++) {
		value = rand();
		printf("key: %i \n", keys[i]);
		printf("value: %i \n", value);
		clock_gettime(CLOCK_REALTIME, &time_before);
		setServer(keys[i], value, (struct sockaddr *) &their_addr, sizeof their_addr , sockfd);
		clock_gettime(CLOCK_REALTIME, &time_after);
		calulate_duration(duration_pointer , time_before, time_after);
		// printf("Duration: %ld\n", (*duration_pointer));
		duration_pointer += 1;
	}

	for (i = 0; i < n; i++) {
		clock_gettime(CLOCK_REALTIME, &time_before);
		getServer(keys[i], (struct sockaddr *) &their_addr, sizeof their_addr , sockfd);
		clock_gettime(CLOCK_REALTIME, &time_after);
		calulate_duration(duration_pointer , time_before, time_after);
		// printf("Duration: %ld\n", (*duration_pointer));
		duration_pointer += 1;
	}

	for (i = 0; i < n; i++) {
		clock_gettime(CLOCK_REALTIME, &time_before);
		delServer(keys[i], (struct sockaddr *) &their_addr, sizeof their_addr , sockfd);
		clock_gettime(CLOCK_REALTIME, &time_after);
		calulate_duration(duration_pointer , time_before, time_after);
		// printf("Duration: %ld\n", (*duration_pointer));
		duration_pointer += 1;
	}


	for (i = 0; i < n; i++) {
		clock_gettime(CLOCK_REALTIME, &time_before);
		getServer(keys[i], (struct sockaddr *) &their_addr, sizeof their_addr , sockfd);
		clock_gettime(CLOCK_REALTIME, &time_after);
		calulate_duration(duration_pointer , time_before, time_after);
		// printf("Duration: %ld\n", (*duration_pointer));
		duration_pointer += 1;
	}


	float sum = 0;
	for (i = 0; i < 4 * n; i++) {
		sum += (float) durations[i];
	}
	sum = (sum / (4.0f * n)) / 1000000.0f;

	printf("Mean Duration over 100 requests: %f ms\n", sum);

	/* ******************************************************************
	   Close socket
	 ******************************************************************* */
	close(sockfd);

	return 0;
}
