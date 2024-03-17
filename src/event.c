#include "event.h"
#include <stdlib.h>

event *create_event(event_t type, int etime, customer *c) {
    event *e = (event*)malloc(sizeof(event));
    e->type = type;
    e->etime = etime;
    e->c = c;
    e->v = NULL;
    return e;   
}

event *create_arrival(int etime, customer *c) {
    return create_event(EVENT_ARRIVAL, etime, c);
}

event *create_departure(int etime, customer *c, vendor *v) {
    event *departure = create_event(EVENT_DEPARTURE, etime, c);
    departure->v = v;
    return departure;
}

void free_event(event *e) {
    free(e);
}
