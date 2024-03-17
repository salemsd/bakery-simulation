#ifndef PRIOQUEUE_H
#define PRIOQUEUE_H
#include "event.h"

typedef struct _prioqueue prioqueue;

/**
 * Create and return a pointer to a new priority queue.
 */
prioqueue *create_pq();

/**
 * Empty the priority queue and free the associated memory.
 */
void free_pq(prioqueue *q);

/**
 * Return the current size of the priority queue.
 */
int size_pq(prioqueue *q);

/**
 * Insert an event in the priority queue.
 */
void insert_pq(prioqueue *q, event *e);

/**
 * Remove and a return a pointer to the event with the 
 * lowest even time.
 */
event *remove_min_pq(prioqueue *q);

#endif /* PRIOQUEUE_H */
