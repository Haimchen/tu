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


int packData(char *buffer, char *order, unsigned int key, int value) {
  snprintf(buffer, sizeof order, "%s", order);
  buffer[4] = key >> 8;
  buffer[5] = key;
  buffer[6] = value >> 8;
  buffer[7] = value;

  return 0;
}

void unpack(char *buffer, unsigned int *key, int *value) {
  *key = (buffer[4] << 8) | buffer[5];
  *value = (buffer[6] << 8) | buffer[7];
}

int getServer(int key, char * serverAdress, int serverPort) {

  int sockfd;
  struct sockaddr_in their_addr; // connector's address information
  struct hostent *he;
  fd_set readfds;
  struct timeval tv;
      
      
  //Resolve hostname to IP Address
  if ((he=gethostbyname(serverAdress)) == NULL) {  // get the host info
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

  char order[8];
  packData(order, "GET ", key, 0);
  char buffer[8];

  int return_val, fdmax;
  FD_ZERO(&readfds);
  FD_SET(sockfd, &readfds);
  fdmax = sockfd + 1;


  return_val = -1;


  time_t start;
  time_t now;
  
  start = time(NULL);
    
  double diff;

  do {

    tv.tv_sec = 0;
    tv.tv_usec = 5;
    sendto(sockfd, order, sizeof(order), 0, (struct sockaddr *) &their_addr, sizeof their_addr);
    return_val = select(fdmax, &readfds, NULL, NULL, &tv);

    now = time(NULL);
    diff = difftime(now, start);

  } while ( diff  < 2);
        
  if (return_val == 0) {  
    printf("No Connection to Server\n");
  } else if (return_val < 0) {
    printf("Error receiving data\n");
  } else {
    recv(sockfd, buffer, sizeof buffer, 0);

    if (buffer[0] == 'N' && buffer[1] == 'O' && buffer[2] == 'F') {
      printf("Not found\n");
      return -1;
    } else {
      int key_in = 0;
      int value_in = 0;

      unpack(buffer, &key_in, &value_in);

      printf("Returned: key = %d | value = %d\n", key_in, value_in);
      return 0;
    }
  }
  close(sockfd);
}

int setServer(unsigned int key, int value, char * serverAdress, int serverPort) {

  int sockfd;
  struct sockaddr_in their_addr; // connector's address information
  struct hostent *he;
  fd_set readfds;
  struct timeval tv;
    
    
  //Resolve hostname to IP Address
  if ((he=gethostbyname(serverAdress)) == NULL) {
    herror("gethostbyname");
    exit(1);
  }

  // create new Socket
  sockfd = socket(PF_INET, SOCK_DGRAM, 0);

  //setup transport address
  their_addr.sin_family = AF_INET;
  their_addr.sin_port = htons(serverPort);
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

  char order[8];
  packData(order, "SET ", key, value);
  char buffer[8];

  int return_val, fdmax;
  FD_ZERO(&readfds);
  FD_SET(sockfd, &readfds);
  fdmax = sockfd + 1;


  return_val = -1;


  time_t start;
  time_t now;

  start = time(NULL);

  double diff;

  do {

    tv.tv_sec = 0;
    tv.tv_usec = 5;
    sendto(sockfd, order, sizeof(order), 0, (struct sockaddr *) &their_addr, sizeof their_addr);
    return_val = select(fdmax, &readfds, NULL, NULL, &tv);

    now = time(NULL);
    diff = difftime(now, start);

  } while ( diff  < 2);

  if (return_val == 0) {  
    printf("No Connection to Server\n");
  } else if (return_val < 0) {
    printf("Error receiving data\n");
  } else {
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
  close(sockfd);
}

int delServer(int key, char * serverAdress, int serverPort) {

  int sockfd;
  struct sockaddr_in their_addr; // connector's address information
  struct hostent *he;
  fd_set readfds;
  struct timeval tv;
      
      
  //Resolve hostname to IP Address
  if ((he=gethostbyname(serverAdress)) == NULL) {
    herror("gethostbyname");
    exit(1);
  }


  sockfd = socket(PF_INET, SOCK_DGRAM, 0);

  //setup transport address
  their_addr.sin_family = AF_INET;
  their_addr.sin_port = htons(serverPort);
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

  char order[8];
  packData(order, "DEL ", key, 0);
  char buffer[8];

  int return_val, fdmax;
  FD_ZERO(&readfds);
  FD_SET(sockfd, &readfds);
  fdmax = sockfd + 1;


  return_val = -1;


  time_t start;
  time_t now;
  
  start = time(NULL);
  
  double diff;

  do {
    tv.tv_sec = 0;
    tv.tv_usec = 5;
    sendto(sockfd, order, sizeof(order), 0, (struct sockaddr *) &their_addr, sizeof their_addr);
    return_val = select(fdmax, &readfds, NULL, NULL, &tv);

    now = time(NULL);
    diff = difftime(now, start);

  } while ( diff  < 2);
      
  if (return_val == 0) {  
    printf("No Connection to Server\n");
  } else if (return_val < 0) {
    printf("Error receiving data\n");
  } else {
    recv(sockfd, buffer, sizeof buffer, 0);

    if (buffer[0] == 'O' && buffer[1] == 'K' && buffer[2] == '!') {
      printf("Removed\n");
      return 0;
    } else {
    printf("An error happens! Value not removed.\n");
    return -1;
    }
  }
  close(sockfd);
}


int main(int argc, char *argv[]) {
  int serverPort;
  unsigned int i;
  unsigned int n = 4;
  int a = 0;


  printf("TCP client example\n\n");

  if (argc != 3) {
    fprintf(stderr,"Usage: tcpClient serverName serverPort\n");
    exit(1);
  }

  serverPort = atoi(argv[2]);
  char* serverAdress = (argv[1]);


  srand(time(NULL));

  for (i = 0; i < n; i++) {
    a = rand();
    setServer(i, a, serverAdress, serverPort);
  } 

  for (i = 0; i < n; i++) {
    getServer(i, serverAdress, serverPort);
  }
  
  for (i = 0; i < n; i++) {
    delServer(i, serverAdress, serverPort);
  }


  for (i = 0; i < n; i++) {
    getServer(i, serverAdress, serverPort);
  }

   return 0;
}
