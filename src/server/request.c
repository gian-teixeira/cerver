#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <omp.h>

struct request {
    struct sockaddr_in client;
    int client_sock;
    int client_size;
    int server_socket;
};

struct request *request_create(int server_socket) 
{
    struct request *request = malloc(sizeof(struct request));
    request->client_size = sizeof(request->client);
    request->server_socket = server_socket;
    request->client_sock = -1;
    return request;
}

struct request *request_wait(int server_socket) 
{
    struct request *request = request_create(server_socket);
    request->client_sock = accept(request->server_socket, 
                                  (struct sockaddr*)&(request->client), 
                                  &(request->client_size));
    return request;
}

void request_response(struct request *request) 
{
    char buffer[100];
    recv(request->client_sock, buffer, sizeof(buffer), 0);
    sprintf(buffer, "%d\n", omp_get_thread_num());
    send(request->client_sock, buffer, strlen(buffer), 0);
    free(request);
}

int request_get_client(struct request *request)
{
    return request->client_sock;
}