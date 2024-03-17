#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "event.h"
#include "customer.h"
#include "vendor.h"
#include "queue.h"
#include "prioqueue.h"

#define N_VENDORS 3
#define ARRIVAL_RATE (1.0/60)
#define MEAN_SERVICE_TIME 150
#define CLOSING_TIME 28800

prioqueue*        event_queue;
queue*            customer_queue;
vendor_list       available_vendors;

int current_time = 0;
int customer_count = 0;
int total_time = 0;

double normal_delay(double mean) {
    return -mean*log(1 - ((double)rand()/RAND_MAX));
}

double factorial(int n) {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

double erlang_C(int m, double E) {    
    double sum = 0;
    for (int i = 0; i < m; i++) {
        sum += pow(E, i) / factorial(i);
    }

    double numerator = (pow(E, m) / factorial(m)) * (m / m - E);
    double denominator = sum + numerator;

    return numerator / denominator;
}

double calculate_response_time() {
    double numerator = erlang_C(N_VENDORS, (1/ARRIVAL_RATE) / (double)MEAN_SERVICE_TIME);
    double lambda = 1/ARRIVAL_RATE;
    double mu = MEAN_SERVICE_TIME;
    double left_side = numerator / (lambda * mu - N_VENDORS);

    return left_side + 1.0/mu;
}

void add_customer(customer *c) {
    // If no vendor is available, add the customer to the queue
    if (available_vendors == NULL) {
        enqueue_q(customer_queue, c);
        return;
    }

    // If a vendor is available, assign the customer to the vendor and create a departure event
    vendor *v = pop_vendor(&available_vendors);

    v->c = c;
    event *new_departure = create_departure(current_time + normal_delay(MEAN_SERVICE_TIME), c, v);
    insert_pq(event_queue, new_departure);
}

void remove_customer(customer *c, vendor *v) {
    v->c = NULL;
    free_customer(c);
    // If there are customers in the queue, assign the first one to the vendor
    if (size_q(customer_queue) > 0) {
        customer *next_customer = dequeue_q(customer_queue);
        v->c = next_customer;
        event *new_departure = create_departure(current_time + normal_delay(MEAN_SERVICE_TIME), next_customer, v);
        insert_pq(event_queue, new_departure);

        return;
    }

    // If there are no customers in the queue, add the vendor to the available vendors list
    insert_vendor(&available_vendors, v);
}

void process_arrival(event *e) {
    customer *new_customer = create_customer(current_time + normal_delay(1.0/ARRIVAL_RATE));
    event *new_arrival = create_arrival(new_customer->atime, new_customer);
    insert_pq(event_queue, new_arrival);
}

void process_departure(event *e) {
    total_time += e->etime - e->c->atime;
    remove_customer(e->c, e->v);
    customer_count++;
}

void display_bakery() {
    printf("%8d | ", current_time);
    
    int available_vendors_count = vendor_list_size(available_vendors);
    
    for (int j = 0; j < N_VENDORS - available_vendors_count; j++) {
        printf("X");
    }
    for (int j = 0; j < available_vendors_count; j++) {
        printf("_");
    }
    
    printf(" | ");
    
    for (int j = 0; j < size_q(customer_queue); j++) {
        printf("X");
    }
    
    printf("\n");
}

void process_events(prioqueue *event_queue) {
    while (size_pq(event_queue) > 0) {
        event *e = remove_min_pq(event_queue);
        current_time = e->etime;

        if (e->type == EVENT_ARRIVAL) {
            add_customer(e->c);
            if (current_time < CLOSING_TIME) {
                process_arrival(e);
            }
        } else if (e->type == EVENT_DEPARTURE) {
            process_departure(e);
        }
        
        free_event(e);

        display_bakery();
    }
}

int main() {
    // Initialize the event queue and customer queue
    event_queue = create_pq();
    display_pq(event_queue);
    customer_queue = create_q();
    
    for (int i = 0; i < N_VENDORS; i++) {
        insert_vendor(&available_vendors, create_vendor(NULL));
    }

    customer *customer_one = create_customer(60);
    event *arrival_one = create_arrival(customer_one->atime, customer_one);
    insert_pq(event_queue, arrival_one);

    // Event loop
    process_events(event_queue);

    printf("----\nCustomer count: %d\navg_time: %f\n", customer_count, (double)total_time/customer_count);

    free_vendor_list(available_vendors);
    free_q(customer_queue);
    free_pq(event_queue);

    return 0;
}
