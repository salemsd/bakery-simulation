#ifndef VENDOR_H
#define VENDOR_H

#include "customer.h"

typedef struct s_vendor_node {
    customer *c;                /* currently served customer */
    struct s_vendor_node *next; /* next vendor */
} vendor, *vendor_list;

/**
 * Create and return a pointer to a new vendor.
 * c: currently served customer
 */
vendor *create_vendor(customer *c);

/**
 * Insert a vendor at the beginning of the list
 * v_list: pointer to the list
 * v: vendor to insert
 */
void insert_vendor(vendor_list *v_list, vendor *v);

/**
 * Remove and return the first vendor in the list
 */
vendor *pop_vendor(vendor_list *v_list);

/**
 * Return the number of vendors in the list
 */
int vendor_list_size(vendor_list v_list);

/**
 * Display the list of vendors
 */
void display_vendor_list(vendor_list v_list);

/**
 * Free the memory associated with a vendor
 */
void free_vendor(vendor *v);

/**
 * Free the memory associated with a vendor list
 */
void free_vendor_list(vendor_list v_list);

#endif /* VENDOR_H */
