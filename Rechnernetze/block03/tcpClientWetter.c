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

int main(int argc, char *argv[]) {
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd;

    
    if (argc != 1) {
        fprintf(stderr,"Too many arguments\n");
        exit(1);
    }

    char *adress = "weather.yahooapis.com"; 
    char order[(sizeof adress) + 70];
    snprintf(order, sizeof order, "GET /forecastrss?w=638242&u=c HTTP/1.1\nHOST: %s\n\n", adress);
    char *port = "80";
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    getaddrinfo(adress, port, &hints, &res);
    
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    
    connect(sockfd, res->ai_addr, res->ai_addrlen);

    write(sockfd, order, sizeof order);
    
    char buffer[20];
    
    int count = 0;
    char *tempStart;
    char *tempEnd;

    do {
        memcpy(&buffer[0], &buffer[10], 10);
        count = recv(sockfd, &buffer[10], 10, 0);

        tempStart = strstr(buffer, "temp=\"");

        if (tempStart != NULL) {
            tempStart += 6;
            if (&buffer[10] > tempStart) {
                
                tempEnd = strstr(tempStart, "\"");
                *tempEnd = 0;
                printf("The current Temperature in Berlin is %s°C.\n", tempStart);
                break;

            }
        }
    } while (count > 0);

    close(sockfd);

    return 0;
}
