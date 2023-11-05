#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct queue queue_t;

struct queue;

struct queue *queue_create();
void queue_destroy(struct queue* q);
int queue_empty(struct queue *q);
void *queue_front(struct queue *q);
void *queue_back(struct queue *q);
void queue_push(struct queue *q, void *data);
void queue_pop(struct queue* q);

#endif