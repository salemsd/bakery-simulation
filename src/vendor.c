#include "vendor.h"
#include "customer.h"

#include <stdlib.h>

vendor *create_vendor(customer *c) {
    vendor *v = (vendor*)malloc(sizeof(vendor));
    v->c = NULL;
    return v;
}

void insert_vendor(vendor_list *v_list, vendor *v) {
    v->next = *v_list;
    *v_list = v;
}

vendor *pop_vendor(vendor_list *v_list) {
    if (*v_list == NULL) {
        return NULL;
    }

    vendor *v = *v_list;
    *v_list = v->next;

    return v;
}

int vendor_list_size(vendor_list v_list) {
    int size = 0;
    for (vendor *v = v_list; v != NULL; v = v->next) {
        size++;
    }
    return size;
}

void display_vendor_list(vendor_list v_list) {
    for (vendor *v = v_list; v != NULL; v = v->next) {
        printf("%p -> ", (void*)v);
    }
    printf("NULL\n");
}

void free_vendor(vendor *v) {
    free(v);
}

void free_vendor_list(vendor_list v_list) {
    for (vendor *v = v_list; v != NULL; v = v->next) {
        free_vendor(v);
    }
}