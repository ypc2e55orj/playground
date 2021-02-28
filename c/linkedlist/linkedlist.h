#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

typedef int LINKEDLIST_DATA_TYPE;

typedef void (*linkedlist_data_handler_t)(LINKEDLIST_DATA_TYPE *);
typedef int (*linkedlist_data_compare_t)(LINKEDLIST_DATA_TYPE *, LINKEDLIST_DATA_TYPE *);

typedef struct __node__
{
    LINKEDLIST_DATA_TYPE *data;
    struct __node__ *prev;
    struct __node__ *next;
} node_t;

typedef struct __linkedlist__
{
    node_t *head;
    node_t *tail;
} linkedlist_t;

linkedlist_t *linkedlist_initialize();
void linkedlist_from_head(linkedlist_t *, linkedlist_data_handler_t);
void linkedlist_from_tail(linkedlist_t *, linkedlist_data_handler_t);
// success: 0, failed: -1, 1
int linkedlist_insert_before(linkedlist_t *, node_t *, LINKEDLIST_DATA_TYPE *);
// success: 0, failure: 1
int linkedlist_add_head(linkedlist_t *, LINKEDLIST_DATA_TYPE *);
// success: 0, failed: -1, 1
int linkedlist_insert_after(linkedlist_t *, node_t *, LINKEDLIST_DATA_TYPE *);
// success: 0, failure: 1
int linkedlist_add_tail(linkedlist_t *, LINKEDLIST_DATA_TYPE *);
LINKEDLIST_DATA_TYPE *linkedlist_remove(linkedlist_t *, node_t *);
LINKEDLIST_DATA_TYPE *linkedlist_remove_head(linkedlist_t *);
LINKEDLIST_DATA_TYPE *linkedlist_remove_tail(linkedlist_t *);
node_t *linkedlist_eq_node_search(node_t *, linkedlist_data_compare_t, LINKEDLIST_DATA_TYPE *);
void linkedlist_free(linkedlist_t *, linkedlist_data_handler_t);

#endif // LINKEDLIST_H