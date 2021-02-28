#ifndef BINARY_TREE_H
#define BINARY_TREE_H

typedef int binary_tree_data_t;

typedef struct binary_tree_node
{
    binary_tree_data_t *data;
    struct binary_tree_node *left;
    struct binary_tree_node *right;
} binary_tree_node_t;

typedef void (*binary_tree_node_handler_t)(binary_tree_node_t *);

typedef void (*binary_tree_data_handler_t)(binary_tree_data_t *);
typedef int (*binary_tree_data_compare_t)(binary_tree_data_t *, binary_tree_data_t *);

binary_tree_node_t *binary_tree_new_node();

// success: 0, faild: NULL(-1), duplicate(1)
int binary_tree_add_node(
    binary_tree_node_t *,
    binary_tree_data_t *,
    binary_tree_data_compare_t);

void binary_tree_in_order(
    binary_tree_node_t *,
    binary_tree_node_handler_t);

void binary_tree_pre_order(
    binary_tree_node_t *,
    binary_tree_node_handler_t);

void binary_tree_post_order(
    binary_tree_node_t *,
    binary_tree_node_handler_t);

void binary_tree_finalize_node(
    binary_tree_node_t *,
    binary_tree_data_handler_t);

#endif // BINARY_TREE_H
