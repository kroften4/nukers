#ifndef _TS_QUEUE_H
#define _TS_QUEUE_H

/*
 * Thread safe FIFO queue implementation (using linked list)
 */

#include <pthread.h>
#include <stddef.h>
#include <stdbool.h>

#define TS_QUEUE(name, size) struct ts_queue name = {.capacity = size};\
                             ts_queue_init(&name)

struct ts_queue_node {
    void *data;
    struct ts_queue_node *prev;
    struct ts_queue_node *next;
};

struct ts_queue_node *ts_queue_node_new(void);

/*
 * Thread safe queue.
 *
 * `data_destructor` is noop by default, optionally provide a custom data
 * destructor to call on node destruction
 */
struct ts_queue {
    struct ts_queue_node *head;
    struct ts_queue_node *tail;
    size_t size;
    void (*data_destructor)(void *data);
    pthread_mutex_t mutex;
};

/*
 * Allocate new ts_queue
 */
struct ts_queue *ts_queue_new(void);

void __ts_queue_data_destructor_noop(void *dummy);

void __ts_queue_node_destructor(struct ts_queue *q, 
                                struct ts_queue_node *node);

/*
 * Free all nodes. To automatically free the data inside them provide a
 * `q->data_destructor`. Freeing q is on you
 */
void ts_queue_destroy(struct ts_queue *q);

void __ts_queue_add(struct ts_queue *q, struct ts_queue_node *prev,
                    struct ts_queue_node *next, struct ts_queue_node *node);

void __ts_queue_remove_nolock(struct ts_queue *q, struct ts_queue_node *prev,
                    struct ts_queue_node *next);

void ts_queue_remove(struct ts_queue *q, struct ts_queue_node *prev,
                    struct ts_queue_node *next);

/*
 * Only for internal use. Not thread safe unless `q->mutex` explicitly locked
 */
bool __ts_queue_is_empty(struct ts_queue *q);

void __ts_queue_enqueue_nolock(struct ts_queue *q, void *item);

void ts_queue_enqueue(struct ts_queue *q, void *item);

void __ts_queue_dequeue_nolock(struct ts_queue *q);

void ts_queue_dequeue(struct ts_queue *q);

#endif
