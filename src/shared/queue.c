#include "queue.h"
#include "stdlib.h"
#include "string.h"

struct queue_data {
    struct queue_data *next;
    void *data;
};

struct queue {
    struct queue_data *head;
    struct queue_data *back;
};

struct queue *queue_create()
{
    struct queue *q = malloc(sizeof(struct queue));
    q->head = malloc(sizeof(struct queue_data));
    q->head->next = NULL;
    q->back = q->head;
    return q;
}

void queue_destroy(struct queue *q) 
{
    struct queue_data *tmp;
    while(tmp = q->head) {
        q->head = q->head->next;
        free(tmp);
    }
    free(q);
}

int queue_empty(struct queue *q) 
{
    return (q->head->next == NULL);
}

void *queue_front(struct queue *q)
{
    if(queue_empty(q)) return NULL;
    return q->head->next->data;
}

void *queue_back(struct queue *q)
{
    return q->back->data;
}

void queue_push(struct queue *q, void *data)
{
    struct queue_data *qd = malloc(sizeof(struct queue_data));
    qd->data = data;
    qd->next = NULL;
    q->back->next = qd;
    q->back = qd;
}

void queue_pop(struct queue *q)
{
    if(queue_empty(q)) return;
    struct queue_data *tmp = q->head->next;
    if(q->back == q->head->next) q->back = q->head;
    q->head->next = tmp->next;
    free(tmp);
}