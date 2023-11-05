#ifndef __SERVER_H_
#define __SERVER_H_

typedef struct server server_t;

struct server;

struct server *server_create(char *ip, int port);
void server_destroy(struct server *server);
void server_init(struct server *server);
void server_close(struct server *server);

#endif