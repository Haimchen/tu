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

void prettifyHttpResponse(char response[]);

void httpRequest(struct addrinfo *addr, char request[], int requestSize, char response[], int responseSize) {
    int sockfd;

    sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    connect(sockfd, addr->ai_addr, addr->ai_addrlen);
    write(sockfd, request, requestSize);
    memset(response, 0, responseSize);
    recv(sockfd, response, responseSize, 0);
    prettifyHttpResponse(response);
    close(sockfd);
    // sleep(1);
}

void prettifyHttpResponse(char response[]) {
    char *position = strstr(response, " ");
    if (position == NULL) {
        printf("No response received\n");
        return;
    }
    char statusCode[4];
    memset(statusCode, 0, 4);
    memcpy(statusCode, ++position, 3);

    position = strstr(response, "\r\n\r\n");
    if (position != NULL)
    {
        position += 4;
        printf("Status %s: %s\n", statusCode, position);
    } else {
        printf("Status %s\n", statusCode);
    }
}

int main(int argc, char *argv[]) {
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd;

    
    if (argc != 3) {
        fprintf(stderr,"Usage: httpClient ServerAdress Port\n");
        exit(1);
    }

    char *adress = argv[1]; 
    char *port = argv[2];

    char request[(sizeof adress) + 512];
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    getaddrinfo(adress, port, &hints, &res);
    
    

    const int N = 3;
    uint16_t keys[N];
    uint16_t value;
    
    char buffer[1000];


    srand(time(NULL));

    int i;
    for (i = 0; i < N; ++i)
    {
        keys[i] = rand();
    }    

    for (i = 0; i < N; ++i)
    {
        value = rand();
        snprintf(buffer, sizeof buffer, "value=%d", value);
        snprintf(request, sizeof request, "POST /hashtable/%d HTTP/1.1\nHost: %s\nContent-Length: %lu\nContent-Type: application/x-www-form-urlencoded\n\n%s", keys[i], adress, strlen(buffer), buffer);
        printf("POST key=%i, value =%i\n", keys[i], value);
        httpRequest(res, request, sizeof request, buffer, sizeof buffer);
    }     
    
    for (i = 0; i < N; ++i)
    {
        snprintf(request, sizeof request, "GET /hashtable/%d HTTP/1.1\nHost: %s\n\n", keys[i], adress);
        printf("GET key=%i\n", keys[i]);
        httpRequest(res, request, sizeof request, buffer, sizeof buffer);
    }      


    for (i = 0; i < N; ++i)
    {
        snprintf(request, sizeof request, "DELETE /hashtable/%d HTTP/1.1\nHost: %s\n\n", keys[i], adress);
        printf("DELETE key=%i\n", keys[i]);
        httpRequest(res, request, sizeof request, buffer, sizeof buffer);
    }

    for (i = 0; i < N; ++i)
    {
        snprintf(request, sizeof request, "GET /hashtable/%d HTTP/1.1\nHost: %s\n\n", keys[i], adress);
        printf("GET key=%i\n", keys[i]);
        httpRequest(res, request, sizeof request, buffer, sizeof buffer);
    }  

    return 0;
}
