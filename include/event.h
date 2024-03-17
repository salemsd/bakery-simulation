#ifndef EVENT_H
#define EVENT_H
#include "customer.h"
#include "vendor.h"

typedef enum {
    EVENT_ARRIVAL,
    EVENT_DEPARTURE
} event_t;

typedef struct {
    event_t     type;   /* event type */
    int         etime;  /* event time */
    customer    *c;     /* customer pointer  */
    vendor      *v;     /* vendor pointer */
} event;

/**
 * Create and return a pointer to a new arrival event.
 * etime: event time
 */
event *create_arrival(int etime, customer *c);

/**
 * Create and return a pointer to a new departure event.
 * etime: event time
 * c: departing customer
 * v: vendor serving the customer
 */
event *create_departure(int etime, customer *c, vendor *v);

/**
 * Free the memory associated with an event.
 */
void free_event(event *e);

#endif /* EVENT_H */
