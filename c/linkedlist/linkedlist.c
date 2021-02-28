#include "linkedlist.h"

linkedlist_t *linkedlist_initialize()
{
    linkedlist_t *list = (linkedlist_t *)malloc(sizeof(linkedlist_t));
    if (list == NULL)
        return NULL;

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void linkedlist_from_head(linkedlist_t *list, linkedlist_data_handler_t handler)
{
    node_t *current = list->head;

    while (current != NULL)
    {
        handler(current->data);
        current = current->next;
    }
}

void linkedlist_from_tail(linkedlist_t *list, linkedlist_data_handler_t handler)
{
    node_t *current = list->tail;

    while (current != NULL)
    {
        handler(current->data);
        current = current->prev;
    }
}

// success: 0, failed: -1, 1
int linkedlist_insert_before(linkedlist_t *list, node_t *cursor, LINKEDLIST_DATA_TYPE *data)
{
    if (cursor == NULL)
        return -1;

    node_t *insert_node = (node_t *)malloc(sizeof(node_t));
    if (insert_node == NULL)
        return 1;

    insert_node->data = data;

    insert_node->prev = cursor->prev;
    insert_node->next = cursor;

    if (cursor->prev != NULL)
        cursor->prev->next = insert_node;

    cursor->prev = insert_node;

    if (list->head == cursor)
        list->head = insert_node;

    return 0;
}

// success: 0, failure: 1
int linkedlist_add_head(linkedlist_t *list, LINKEDLIST_DATA_TYPE *data)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL)
        return 1;

    node->prev = NULL;
    node->data = data;

    if (list->head == NULL)
    {
        list->tail = node;
    }
    else
    {
        list->head->prev = node;
    }

    node->next = list->head;
    list->head = node;

    return 0;
}

// success: 0, failed: -1, 1
int linkedlist_insert_after(linkedlist_t *list, node_t *cursor, LINKEDLIST_DATA_TYPE *data)
{
    if (cursor == NULL)
        return -1;

    node_t *insert_node = (node_t *)malloc(sizeof(node_t));
    if (insert_node == NULL)
        return 1;

    insert_node->data = data;
    insert_node->prev = cursor;
    insert_node->next = cursor->next;

    if (cursor->next != NULL)
        cursor->next->prev = insert_node;

    cursor->next = insert_node;

    if (list->tail == cursor)
        list->tail = insert_node;

    return 0;
}

// success: 0, failure: 1
int linkedlist_add_tail(linkedlist_t *list, LINKEDLIST_DATA_TYPE *data)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL)
        return 1;

    node->next = NULL;
    node->data = data;

    if (list->tail == NULL)
    {
        list->head = node;
    }
    else
    {
        list->tail->next = node;
    }

    node->prev = list->tail;
    list->tail = node;

    return 0;
}

LINKEDLIST_DATA_TYPE *linkedlist_remove(linkedlist_t *list, node_t *remove_node)
{
    if (remove_node == NULL)
        return NULL;

    LINKEDLIST_DATA_TYPE *data = remove_node->data;

    if (list->head == remove_node)
        list->head = remove_node->next;

    if (list->tail == remove_node)
        list->tail = remove_node->prev;

    if (remove_node->prev != NULL)
        remove_node->prev->next = remove_node->next;

    if (remove_node->next != NULL)
        remove_node->next->prev = remove_node->prev;

    free(remove_node);

    return data;
}

LINKEDLIST_DATA_TYPE *linkedlist_remove_head(linkedlist_t *list)
{
    return linkedlist_remove(list, list->head);
}

LINKEDLIST_DATA_TYPE *linkedlist_remove_tail(linkedlist_t *list)
{
    return linkedlist_remove(list, list->tail);
}

node_t *linkedlist_eq_node_search(node_t *from, linkedlist_data_compare_t compare, LINKEDLIST_DATA_TYPE *data)
{
    if (from == NULL)
        return NULL;

    node_t *current = from;

    while (compare(data, current->data) != 0)
    {
        if (current->next == NULL)
            return NULL;

        current = current->next;
    }

    return current;
}

void linkedlist_free(linkedlist_t *list, linkedlist_data_handler_t handler)
{
    if (list == NULL)
        return;

    node_t *current = list->head;
    node_t *next;

    while (current != NULL)
    {
        next = current->next;
        handler(current->data);
        free(current);
        current = next;
    }

    free(list);
}