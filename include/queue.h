#ifndef QUEUE_H
#define QUEUE_H
#include "customer.h"

typedef struct _link link;
typedef struct _queue queue;

/**
 * Create and return a pointer to a new queue.
 */
queue *create_q();

/**
 * Empty the queue and free the associated memory.
 */
void free_q(queue *q);

/**
 * Return the current size of the queue.
 */
int size_q(queue *q);

/**
 * Add a customer to the end of the queue.
 */
void enqueue_q(queue *q, customer *c);

/**
 * Create and return a pointer to a new link.
 */
link *create_link(customer *c, link *next);

/**
 * Display the queue.
 */
void display_q(queue *q);


/**
 * Remove and a return a pointer to the first customer in the queue.
 */
customer *dequeue_q(queue *q);

#endif /* QUEUE_H */
