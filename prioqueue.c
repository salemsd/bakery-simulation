#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "prioqueue.h"
#include "event.h"

typedef struct _link {
    event        *e;
    struct _link *next;
} link;
struct _prioqueue {
    link *first;
    int    size;
};

prioqueue *create_pq() {
    prioqueue *q = (prioqueue*)malloc(sizeof(prioqueue));
    q->first = NULL;
    q->size = 0;
    
    return q;
}

link *create_elink(event *e, link *next) {
    link *l = (link*)malloc(sizeof(link));
    l->e = e;
    l->next = next;

    return l;
}

void free_pq(prioqueue *q) {
    while (size_pq(q) != 0) {
        event *e = remove_min_pq(q);
        e->c != NULL ? free_customer(e->c) : 0;
        free_event(e);
    }
    free(q);
}

int size_pq(prioqueue *q) {
    return q->size;
}

void insert_pq(prioqueue *q, event *e) {
    link *l = create_elink(e, NULL);
    if (l == NULL) {
        fprintf(stderr, "Allocation failed\n");
        exit(1);
    }

    link *prev = NULL;
    link *cur = q->first;
    // Iterate through the queue until we find the right place to insert the new link.
    for (; cur != NULL && e->etime >= cur->e->etime; cur = cur->next) {
        prev = cur;
    }
    // If the queue is empty, or we need to insert before its head
    if (prev == NULL) {
        l->next = q->first;
        q->first = l;
    } else { // Insert the new link between prev and cur (the link with superior event time)
        l->next = cur;
        prev->next = l;
    }

    q->size++;
}

void display_pq(prioqueue *q) {
    link *iterator;
    for (iterator = q->first; iterator != NULL; iterator = iterator->next) {
        printf("event type and time: %d %d --> ", iterator->e->type, iterator->e->etime);
    }
}

event *remove_min_pq(prioqueue *q) {
    if (size_pq(q) <= 0) {
        fprintf(stderr, "Dequeuing empty queue\n");
        exit(1);
    }
    link *l = q->first; // The link to remove
    event *e = l->e; // The event to return
    q->first = l->next; // Update the first pointer
    free(l);
    q->size--;
    
    return e;
}
