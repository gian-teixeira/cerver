#include "server.h"
#include "request.h"
#include "http.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <omp.h>

#define MANAGER_THREAD 0
#define lock_and_run(omp_lock, code) \
    omp_set_lock(&omp_lock); code;  \
    omp_unset_lock(&omp_lock);

struct server {
    int socket;
    struct sockaddr_in addr;
};

void server_handle_request(struct request *request)
{
    char buffer[BUFFER_SIZE];
    memset(buffer,0,sizeof(buffer));

    int client = request_get_client(request);
    recv(client, buffer, sizeof(buffer), 0);
    printf("%s",buffer);
    http_request_t *http_request = http_request_parse(buffer);

    

    sprintf(buffer, "%d\n", omp_get_thread_num());
    send(client, buffer, strlen(buffer), 0);

    http_destroy_request(http_request);
    close(client);
    free(request);
}

void server_loop(struct server *server)
{
    queue_t *request_queue = queue_create();
    omp_lock_t queue_lock;
    omp_init_lock(&queue_lock);

    #pragma omp parallel
    {
        switch (omp_get_thread_num()) {
        case MANAGER_THREAD:
            while(1) {
                listen(server->socket, 5);
                request_t *new_request = request_wait(server->socket);
                lock_and_run(queue_lock, {
                    queue_push(request_queue, new_request);
                });
            }
            break;
        default:
            request_t *request;
            while(1) {
                if(queue_empty(request_queue)) continue;
                lock_and_run(queue_lock, {
                    request = queue_front(request_queue);
                    if(request) queue_pop(request_queue);
                });
                if(request) server_handle_request(request);
            }
        }
    }
}

struct server *server_create(char *ip, int port) 
{
    struct server *server = malloc(sizeof(struct server));

    server->socket = socket(AF_INET, SOCK_STREAM, 0); 

    if(server->socket == -1) {
        perror("[-] Socket create failed");
        exit(1);
    }

    int option = 1;
    setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    memset(&(server->addr), 0, sizeof(server->addr));
    server->addr.sin_family = AF_INET;
    server->addr.sin_addr.s_addr = INADDR_ANY;
    server->addr.sin_port = htons(port);
    
    return server;
}

void server_destroy(struct server *server)
{
    close(server->socket);
    free(server);
}

void server_init(struct server *server)
{
    if(bind(server->socket, 
            (struct sockaddr*)&server->addr,
            sizeof(server->addr)) < 0) {
        perror("[-] Bind failed");
        exit(1);
    }
    printf("Bind to %s:%d\n",
        inet_ntoa(server->addr.sin_addr),
        ntohs(server->addr.sin_port));
    server_loop(server);
}

void server_close(struct server *server)
{
    close(server->socket);
}