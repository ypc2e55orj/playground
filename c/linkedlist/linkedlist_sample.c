#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

void data_print(int *data)
{
    printf("%d ", *data);
}

int data_compare(int *key, int *data)
{
    return *key == *data ? 0 : 1;
}

// success: 0, failure: 1
int getint(int *v)
{
    int scanf_ret = scanf("%d", v);
    if (feof(stdin) || ferror(stdin))
    {
        clearerr(stdin);
        return 1;
    }

    if (scanf_ret != 1)
    {
        // clear stdin
        scanf("%*c");
        return 1;
    }

    return 0;
}

int main(void)
{
    linkedlist_t *list = NULL;
    int *p = NULL;
    int op = 0;
    int key = 0;
    node_t *node = NULL;

    list = linkedlist_initialize();
    if (list == NULL)
        goto memory_alloc_error;

    while (1)
    {
        printf(
            "\n"
            "1=reset, "
            "2=add_head, "
            "3=add_tail, "
            "4=print_from_head"
            "\n"
            "5=print_from_tail, "
            "6=remove_head, "
            "7=remove_tail"
            "\n"
            "8=search, "
            "9=insert, "
            "10=delete"
            "\n"
            "-> ");

        if (getint(&op))
            op = 0;

        switch (op)
        {
        case 1: // reset
            linkedlist_free(list, (void (*)(int *))free);

            list = linkedlist_initialize();
            if (list == NULL)
                goto memory_alloc_error;

            printf("reset\n");

            break;

        case 2: // add_head
            p = (int *)malloc(sizeof(int));
            if (p == NULL)
                goto memory_alloc_error;

            printf("add_head: ");
            if (getint(p))
            {
                free(p);
                break;
            }

            printf(
                linkedlist_add_head(list, p)
                    ? "failure\n"
                    : "success\n");

            break;

        case 3: // add_tail
            p = (int *)malloc(sizeof(int));
            if (p == NULL)
                goto memory_alloc_error;

            printf("add_tail: ");
            if (getint(p))
            {
                free(p);
                break;
            }

            printf(
                linkedlist_add_tail(list, p)
                    ? "failure\n"
                    : "success\n");

            break;

        case 4: // print_from_head
            linkedlist_from_head(list, data_print);
            printf("\n");

            break;

        case 5: // print_from_tail
            linkedlist_from_tail(list, data_print);
            printf("\n");

            break;

        case 6: // remove_head
            printf("remove_head: ");
            p = linkedlist_remove_head(list);
            if (p == NULL)
            {
                printf("nothing\n");
            }
            else
            {
                printf("%d\n", *p);
                free(p);
            }

            break;

        case 7: // remove_tail
            printf("remove_tail: ");
            p = linkedlist_remove_tail(list);
            if (p == NULL)
            {
                printf("nothing\n");
            }
            else
            {
                printf("%d\n", *p);
                free(p);
            }

            break;

        case 8: // search
            printf("search key: ");
            if (getint(&key))
                break;

            node = list->head;

            while (1)
            {
                node = linkedlist_eq_node_search(node, data_compare, &key);

                if (node != NULL)
                {
                    printf(
                        "%d is found"
                        "\n"
                        "find next? (0: y, 1: n): ",
                        *node->data);
                    if (getint(&op))
                        break; // while
                    if (op == 1)
                        break; // while

                    node = node->next;
                }
                else
                {
                    printf("%d is not found\n", key);
                    break; // while
                }
            }

            break;

        case 9: // insert
            if (node == NULL)
            {
                printf("please search node first\n");
                break; // switch
            }
            else
            {
                p = (int *)malloc(sizeof(int));
                if (p == NULL)
                    goto memory_alloc_error;

                printf("enter data to insert: ");
                if (getint(p))
                {
                    free(p);
                    break;
                }

                printf("insert (0: before, 1: after): ");
                if (getint(&op))
                {
                    free(p);
                    break;
                }

                switch (op)
                {
                case 0:
                    printf(
                        linkedlist_insert_before(list, node, p)
                            ? "failure\n"
                            : "success\n");

                    break;

                case 1:
                    printf(
                        linkedlist_insert_after(list, node, p)
                            ? "failure\n"
                            : "success\n");

                    break;

                default:
                    printf("invalid operation");

                    break;
                }
            }

            break;

        case 10: // delete
            if (node == NULL)
            {
                printf("please search node first\n");
                break; // switch
            }
            else
            {
                p = linkedlist_remove(list, node);
                printf("%d was removed\n", *p);
                free(p);
            }

            break;

        default: // free and exit
            linkedlist_free(list, (void (*)(int *))free);
            printf("free and exit\n");

            return 0;
        } // switch(op)
    }     // while(1)

memory_alloc_error:
    printf("memory_alloc_error\n");

    return 1;
}