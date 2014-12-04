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


void unpack(unsigned char *buffer, unsigned int *a, unsigned int *b) {
    *a = (buffer[0] << 8) | buffer[1];
    *b = (buffer[2] << 8) | buffer[3];
}

unsigned int gcd(unsigned int a, unsigned int b) {
  unsigned int c;

  while ( a != 0 ) {
     c = a; a = b%a;  b = c;
  }
  return b;
}

int main(int argc, char *argv[])
{
    int sockfd;
    int sockcl;
    struct sockaddr_in their_addr; // connector's address information
    int serverPort;
    unsigned int their_len;
    unsigned int a = 0;
    unsigned int b = 0;

    printf("TCP Server online\n\n");
    
    if (argc != 2) {
        fprintf(stderr,"Usage: tcpServer serverPort \n");
        exit(1);
    }

    serverPort = atoi(argv[1]);


    /* ******************************************************************
    Create socket
    ******************************************************************* */

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    
    //setup transport address
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(serverPort);
    inet_pton(AF_INET, "127.0.0.1", &(their_addr.sin_addr)); //using localhost as address
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

    /* ******************************************************************
    Binding
    ******************************************************************* */

    their_len = sizeof(their_addr);
    bind(sockfd,(struct sockaddr *) &their_addr, their_len); 
    
    /* ******************************************************************
    Connecting
    ******************************************************************* */

    listen(sockfd, 20);
    
    printf("Listening for Client\n");
    
    sockcl = accept(sockfd, (struct sockaddr *) &their_addr, &their_len);
    
    printf("Connected To Client\n");

    /* ******************************************************************
    Reading transferred data
    ******************************************************************* */
    
    unsigned char buffer[4];
    
    read(sockcl, buffer, sizeof(buffer));
    
    unpack(buffer, &a, &b);
    
    printf("The client returned\na = %d\nb = %d\nGCD = %d\n", a, b, gcd(a, b));

    /* ******************************************************************
    Closing socket
    ******************************************************************* */
    
    close(sockfd);
    
    return 0;
}
