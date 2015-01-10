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

struct timespec diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec - start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp;
}

struct timespec delayFunc(struct timespec time1, struct timespec time2, struct timespec time3, struct timespec time4) {

    //return diff(diff(time4, time1), diff(time3, time2));
    return diff(diff(time2,time3), diff(time1,time4));
}

struct timespec offsetFunc(struct timespec time1,  struct timespec time2, struct timespec time3, struct timespec time4) {
    // return diff(diff(time2, time1), diff(time3, time4)) / 2;
    return 
}

void extractTimeVal(long *timeval, char *src){
    int *intsrc = (int *) src;
    *timeval = (long) ntohl(*intsrc);
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in their_addr; // connector's address information
    socklen_t their_len = sizeof their_addr;
    struct hostent *he;
    int serverPort;

    printf("UDP client example\n\n");

    if (argc != 3) {
        fprintf(stderr,"Usage: tcpClient serverName serverPort\n");
        exit(1);
    }

    serverPort = atoi(argv[2]);

    //Resolve hostname to IP Address
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
    Sending data
    ******************************************************************* */

    char buffer[20];

    const int N = 10;
    int i;

    struct timespec time1, time2, time3, time4, tmp1, tmp2;
  
    int delay[N];
    int offset[N];
    
    for (i = 0; i < N; i++){
        
        clock_gettime(CLOCK_REALTIME, &time1);
	    printf("t1: %lu %lu \n", time1.tv_sec, time1.tv_nsec);
    
        sendto(sockfd, "REQ", 4, 0, (struct sockaddr *) &their_addr, their_len);

        recvfrom(sockfd, buffer, sizeof buffer, 0, (struct sockaddr *) &their_addr, &their_len);
        
        // check if RES!

        clock_gettime(CLOCK_REALTIME, &time4);
        printf("t4: %lu %lu \n", time4.tv_sec, time4.tv_nsec);
        

        extractTimeVal(&time2.tv_sec, &buffer[4]);
        extractTimeVal(&time2.tv_nsec, &buffer[8]);
        extractTimeVal(&time3.tv_sec, &buffer[12]);
        extractTimeVal(&time3.tv_nsec, &buffer[16]);

        printf("t2: %lu %lu \n", time2.tv_sec, time2.tv_nsec);
        printf("t3: %lu %lu \n", time3.tv_sec, time3.tv_nsec);

        tmp1 = delayFunc(time1, time2, time3, time4);
        tmp2 = offsetFunc(time1, time2, time3, time4);
        delay[i] = tmp1.tv_nsec + 1000000000 * tmp1.tv_sec;
        offset[i] = (tmp2.tv_nsec + 1000000000 * tmp1.tv_sec)/2;
        
        printf("delay: %d  ns\n", delay[i]);
        printf("offset: %d ns\n", offset[i]);
    }
    
    int j = 0;
    for (i = 1; i < N; i++){
        if (delay[i] < delay[j]) {
            j = i;   
        }
    }
    printf("The result is:\n");
    printf("delay: %d  ns\n", delay[j]);
    printf("offset: %d ns\n", offset[j]);

    /* ******************************************************************
    Close socket
    ******************************************************************* */

    close(sockfd);

    return 0;
}
