#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <omp.h>
#include "queue.h"

#define PORT 8080

typedef struct request request_t;

struct request {
    struct sockaddr_in client;
    int client_size;
    int connfd;
};

struct request *request_create(int sockfd) {
    struct request *request = malloc(sizeof(struct request));
    request->client_size = sizeof(request->client);
    request->connfd = accept(sockfd, 
                             (struct sockaddr*)&request->client, 
                             &request->client_size);
    return request;
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    char buffer[100] = "request";
    write(sockfd, buffer, sizeof(buffer));
    memset(buffer, 0, sizeof(buffer));
    read(sockfd, buffer, sizeof(buffer));
    printf("%s\n", buffer);

    close(sockfd);

    return 0;
}