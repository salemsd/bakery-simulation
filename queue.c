#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct _link {
    customer*       c;
    struct _link*   next;
} link;

struct _queue {
    link*   first;
    link*   last;
    int     size;
};

queue *create_q() {
    queue *q = (queue*)malloc(sizeof(queue));
    q->first = NULL;
    q->last = NULL;
    q->size = 0;
    return q;
}

link *create_link(customer *c, link *next) {
    link *l = (link*)malloc(sizeof(link));
    l->c = c;
    l->next = next;
    return l;
}

void enqueue_q(queue *q, customer *c) {
    link *l = create_link(c, NULL);
    if (l == NULL) {
        fprintf(stderr, "Allocation failed\n");
        exit(1);
    }
    // If the queue is empty, the new link is both the first and the last.
    if (q->size == 0) {
        q->first = l;
    } else {
        q->last->next = l;
    }
    // Update the last pointer
    q->last = l;
    q->size++;
}

customer *dequeue_q(queue *q) {
    if (q->size <= 0) {
        fprintf(stderr, "Dequeuing empty queue\n");
        exit(1);
    }
    link *l = q->first; // The link to remove
    customer *c = l->c; // The customer to return
    q->first = l->next; // Update the first pointer
    free(l);
    q->size--;
    
    return c;
}

void display_q(queue *q) {
    link *iterator;
    for (iterator = q->first; iterator != NULL; iterator = iterator->next) {
        printf("customer atime: %d --> ", iterator->c->atime);
    }
}

void free_q(queue *q) {
    // Free all the customers first
    for (; q->size != 0; q->size--) {
        customer* c = dequeue_q(q);
        free_customer(c);
    }
    free(q);
}

int size_q(queue *q) {
    return q->size;
}
