#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "event.h"
#include "customer.h"
#include "queue.h"
#include "prioqueue.h"

#define N_VENDORS 3
#define ARRIVAL_RATE (1.0/60)
#define MEAN_SERVICE_TIME 150
#define CLOSING_TIME 28800

prioqueue*  event_queue;
queue*      customer_queue;
customer*   vendor[N_VENDORS];

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
    for (int i = 0; i < N_VENDORS; i++) {
        // Find the first available vendor
        if (vendor[i] == NULL) {
            vendor[i] = c;
            event *new_departure = create_departure(current_time + normal_delay(MEAN_SERVICE_TIME), c);
            insert_pq(event_queue, new_departure);
            return;
        }
    }

    // If no vendor is available, add the customer to the queue
    enqueue_q(customer_queue, c);
}

void remove_customer(customer *c) {
    for (int i = 0; i < N_VENDORS; i++) {
        if (vendor[i] == c) {
            vendor[i] = NULL;
            free_customer(c);
            // If there are customers in the queue, assign the first one to the vendor
            if (size_q(customer_queue) > 0) {
                customer *next_customer = dequeue_q(customer_queue);
                vendor[i] = next_customer;
                event *new_departure = create_departure(current_time + normal_delay(MEAN_SERVICE_TIME), next_customer);
                insert_pq(event_queue, new_departure);
            }
            return;
        }
    }
}

void process_arrival(event *e) {
    customer *new_customer = create_customer(current_time + normal_delay(1.0/ARRIVAL_RATE));
    event *new_arrival = create_arrival(new_customer->atime, new_customer);
    insert_pq(event_queue, new_arrival);
}

void process_departure(event *e) {
    total_time += e->etime - e->c->atime;
    remove_customer(e->c);
    customer_count++;
}

void display_bakery() {
    printf("%8d | ", current_time);
    for (int j = 0; j < N_VENDORS; j++) {
        if (vendor[j] != NULL) {
            printf("X");
        } else {
            printf("_");
        }
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
        vendor[i] = NULL;
    }

    customer *customer_one = create_customer(60);
    event *arrival_one = create_arrival(customer_one->atime, customer_one);
    insert_pq(event_queue, arrival_one);

    // Event loop
    process_events(event_queue);

    printf("----\nCustomer count: %d\navg_time: %f\n", customer_count, (double)total_time/customer_count);

    display_pq(event_queue);
    display_q(customer_queue);
    free_q(customer_queue);
    free_pq(event_queue);

    return 0;
}
