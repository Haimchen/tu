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
#include <time.h>

#define MAX_BUFFER_LENGTH 100

int packData(char buffer[], struct timespec time2, struct timespec time3) {
	
	snprintf(buffer, 4, "%s", "RES");
	unsigned int temp;
    temp = htonl((unsigned int) time2.tv_sec); 
    memcpy(&buffer[4], &temp, 4 );
    temp = htonl((unsigned int) time2.tv_nsec); 
    memcpy(&buffer[8], &temp, 4 );
    temp = htonl((unsigned int) time3.tv_sec); 
    memcpy(&buffer[12], &temp, 4 );
    temp = htonl((unsigned int) time3.tv_nsec); 
    memcpy(&buffer[16], &temp, 4 );
    
    return 0;
}

int main(int argc, char *argv[])
{
    int sockfd;
    int sockcl;
    struct sockaddr_in our_addr; // our address information
    struct sockaddr_in their_addr; // clients address information
    socklen_t their_len = sizeof their_addr;
    int serverPort;
    unsigned int len;

    printf("UDP Server online\n\n");
    
    if (argc != 2) {
        fprintf(stderr,"Usage: udpServer serverPort \n");
        exit(1);
    }

    serverPort = atoi(argv[1]);


    /* ******************************************************************
    TO BE DONE: Create socket
    ******************************************************************* */

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    
    //setup transport address
    our_addr.sin_family = AF_INET;
    our_addr.sin_port = htons(serverPort);
    inet_pton(AF_INET, "0.0.0.0", &(our_addr.sin_addr));
    memset(our_addr.sin_zero, '\0', sizeof our_addr.sin_zero);

    /* ******************************************************************
    TO BE DONE:  Binding
    ******************************************************************* */
    len = sizeof(our_addr);
    bind(sockfd,(struct sockaddr *) &our_addr, len); 
        
    printf("Listening for Client\n");
    
    char request[4];
	unsigned char buffer[20];
	struct timespec time2, time3;

	while(1) {
    	recvfrom(sockfd, request, sizeof request, 0, (struct sockaddr *) &their_addr, &their_len);
		
    	clock_gettime(CLOCK_REALTIME, &time2);
		printf("t2: %lu %lu \n", time2.tv_sec, time2.tv_nsec);

        // checken ob wirklich eine Anfrage (REQ) gesendet wurde
        if(strncmp("REQ", request, 4) != 0){
            printf("ERROR: Unknown Request\n");
            continue;
        }
        // Sleep for testing purposes
        // usleep(100 * 1000);
        
		clock_gettime(CLOCK_REALTIME, &time3);
		printf("t3: %lu %lu \n", time3.tv_sec, time3.tv_nsec);
		
		packData(buffer, time2, time3);
        
    	sendto(sockfd, buffer, sizeof buffer, 0, (struct sockaddr *) &their_addr, their_len);
	}
    close(sockfd);
    
    return 0;
}
