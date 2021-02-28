#include <stdlib.h>
#include "binary_tree.h"

binary_tree_node_t *binary_tree_new_node()
{
    binary_tree_node_t *node =
        (binary_tree_node_t *)malloc(sizeof(binary_tree_node_t));
    if (node == NULL)
        return NULL;

    node->data = NULL;
    node->left = NULL;
    node->right = NULL;

    return node;
}

// success: 0, faild: NULL(-1), duplicate(1)
int binary_tree_add_node(
    binary_tree_node_t *dest,
    binary_tree_data_t *data,
    binary_tree_data_compare_t compare)
{
    if (dest == NULL || data == NULL)
        return -1;

    if (dest->data == NULL)
    {
        dest->data = data;

        return 0;
    }

    int cmp = compare(data, dest->data);

    if (cmp == 0)
        return 1;

    binary_tree_node_t *new_node;
    if (cmp < 0)
    {
        if (dest->left == NULL)
        {
            new_node = binary_tree_new_node();
            if (new_node == NULL)
                return -1;

            new_node->data = data;
            dest->left = new_node;

            return 0;
        }
        else
        {
            return binary_tree_add_node(dest->left, data, compare);
        }
    }
    else
    {
        if (dest->right == NULL)
        {
            new_node = binary_tree_new_node();
            if (new_node == NULL)
                return -1;

            new_node->data = data;
            dest->right = new_node;

            return 0;
        }
        else
        {
            return binary_tree_add_node(dest->right, data, compare);
        }
    }
}

void binary_tree_in_order(
    binary_tree_node_t *node,
    binary_tree_node_handler_t handler)
{
    if (node == NULL)
        return;

    binary_tree_in_order(node->left, handler);
    handler(node);
    binary_tree_in_order(node->right, handler);
}

void binary_tree_pre_order(
    binary_tree_node_t *node,
    binary_tree_node_handler_t handler)
{
    if (node == NULL)
        return;

    handler(node);
    binary_tree_pre_order(node->left, handler);
    binary_tree_pre_order(node->right, handler);
}

void binary_tree_post_order(
    binary_tree_node_t *node,
    binary_tree_node_handler_t handler)
{
    if (node == NULL)
        return;

    binary_tree_post_order(node->left, handler);
    binary_tree_post_order(node->right, handler);
    handler(node);
}

void binary_tree_finalize_node(
    binary_tree_node_t *node,
    binary_tree_data_handler_t handler
)
{
    if (node == NULL)
        return;

    // post order
    binary_tree_finalize_node(node->left, handler);
    binary_tree_finalize_node(node->right, handler);
    handler(node->data);
    free(node);
}
