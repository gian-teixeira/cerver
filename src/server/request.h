#ifndef _REQUEST_H_
#define _REQUEST_H_

typedef struct request request_t;

struct request;

struct request *request_create(int server_socket);
struct request *request_wait(int server_socket);
int request_get_client(struct request *request);
void request_response(struct request *request);

#endif