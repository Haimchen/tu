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


int packData(unsigned char *buffer, unsigned int a, unsigned int b) {
    buffer[0] = a >> 8; // erste Hälfte von a
    buffer[1] = a;      //zweite Hälfte von a
    buffer[2] = b >> 8;
    buffer[3] = b;

    return 0;
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    int serverPort;
    int a = 0;
    int b = 0;

    printf("TCP client example\n\n");

    if (argc != 5) {
        fprintf(stderr,"Usage: tcpClient serverName serverPort int1 int2\n");
        exit(1);
    }

    serverPort = atoi(argv[2]);
    a = atoi(argv[3]);
    b = atoi(argv[4]);

    //Resolv hostname to IP Address
    if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info
        herror("gethostbyname");
        exit(1);
    }

    /* ******************************************************************
    Create socket
    ******************************************************************* */

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    
    //setup transport address
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(serverPort);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

    /* ******************************************************************
    Binding
    ******************************************************************* */

    connect(sockfd, (struct sockaddr *) &their_addr, sizeof(their_addr)); 
    
    unsigned char buffer[4];

    packData(buffer, a, b);

    /* ******************************************************************
    Send data
    ******************************************************************* */

    write(sockfd, buffer, sizeof(buffer));

    /* ******************************************************************
    Close socket
    ******************************************************************* */

    close(sockfd);
    
    return 0;
}
