#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <omp.h>
#include "queue.h"
#include "request.h"
#include "server.h"

#define PORT 8080
#define MANAGER_THREAD 0

#define lock_and_run(omp_lock, code) \
    omp_set_lock(&omp_lock); code;  \
    omp_unset_lock(&omp_lock);



int main()
{
    server_t *server = server_create("127.0.0.1", PORT);
    server_init(server);
    server_close(server);
    server_destroy(server);
    
    /*int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    queue_t *request_queue = queue_create();
    omp_lock_t queue_lock;
    omp_init_lock(&queue_lock);
    
    #pragma omp parallel
    {
        switch (omp_get_thread_num()) {
        case MANAGER_THREAD:
            while(1) {
                listen(server_socket, 5);
                request_t *new_request = request_wait(server_socket);
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
                if(request) request_response(request);
            }
        }
    }

    omp_destroy_lock(&queue_lock);
    close(server_socket);*/

    return 0;
}