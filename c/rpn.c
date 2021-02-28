#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    int *array;
    size_t size;
} int_stack_t;

int_stack_t *int_stack_new()
{
    int_stack_t *stack = (int_stack_t *)malloc(sizeof(int_stack_t));
    if (stack == NULL)
    {
        return NULL;
    }

    stack->size = 0;
    stack->array = (int *)malloc(sizeof(int));
    if (stack->array == NULL)
    {
        free(stack);
        return NULL;
    }

    return stack;
}

// success: 0, failure: 1
int int_stack_push(int_stack_t *stack, int element)
{
    int *realloc_array = (int *)realloc(stack->array, sizeof(int) * ++stack->size);
    if (realloc_array == NULL)
    {
        return 1;
    }

    stack->array = realloc_array;
    stack->array[stack->size - 1] = element;

    return 0;
}

// success: 0, failure: 1
int int_stack_pop(int_stack_t *stack, int *element)
{
    if (stack->size == 0)
    {
        return 1;
    }

    *element = stack->array[--stack->size];

    return 0;
}

size_t int_stack_get_size(int_stack_t *stack)
{
    return stack->size;
}

void int_stack_dump_stderr(int_stack_t *stack)
{
    size_t index;
    for (size_t _index = 0; _index < stack->size; _index++)
    {
        index = stack->size - 1 - _index;
        fprintf(stderr, "[%ld] %d\n", index, stack->array[index]);
    }
}

void int_stack_free(int_stack_t *stack)
{
    free(stack->array);
    free(stack);
}

char *getstring()
{
    char c;
    char *string = (char *)malloc(sizeof(char));
    char *pos = string;
    char *realloc_string;
    size_t length = 1;

    if (string == NULL)
    {
        return NULL;
    }

    while ((c = fgetc(stdin)) != '\n' && !feof(stdin) && !ferror(stdin))
    {
        *pos++ = c;
        realloc_string = (char *)realloc(string, ++length * sizeof(char));
        if (realloc_string == NULL)
        {
            free(string);
            return NULL;
        }

        pos = realloc_string + (pos - string);
        string = realloc_string;
    }
    *pos = '\0';

    clearerr(stdin);

    return string;
}

int isoperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

#define RPN_STACK_PUSH(value)             \
    do                                    \
    {                                     \
        if (int_stack_push(stack, value)) \
        {                                 \
            goto rpn_memory_error;        \
        }                                 \
    } while (0)
// success: 0, failure: 1
int rpn(char *string, int *calc_res)
{
    int_stack_t *stack = int_stack_new();
    if (stack == NULL)
    {
        goto rpn_stack_alloc_error;
    }

    char *pos = string;

    while (isspace(*pos))
    {
        pos++;
    }

    short int disable_multi_digit_mode = 0; // 0: false, 1: true

    char *digits_pos;
    char *digits_string;

    size_t space_count = 0;

    int l_operand, r_operand;

    while (*pos != '\0')
    {
        if (isdigit(*pos)) // digit
        {
            if (!disable_multi_digit_mode)
            {
                for (digits_pos = pos; isdigit(*digits_pos); digits_pos++)
                {
                }

                if (isoperator(*digits_pos) && space_count == 0)
                {
                    disable_multi_digit_mode = 1;
                }
                else
                {
                    digits_string = (char *)malloc(sizeof(char) * (digits_pos - pos + 1));
                    if (digits_string == NULL)
                    {
                        goto rpn_memory_error;
                    }

                    strncpy(digits_string, pos, digits_pos - pos);
                    digits_string[digits_pos - pos] = '\0';

                    l_operand = atoi(digits_string);
                    free(digits_string);

                    RPN_STACK_PUSH(l_operand);

                    pos = --digits_pos;
                }
            }

            if (disable_multi_digit_mode)
            {
                RPN_STACK_PUSH(*pos - '0');
            }
        }
        else if (isoperator(*pos)) // operator
        {
            if (int_stack_pop(stack, &r_operand))
            {
                goto rpn_r_operand_not_found_error;
            }
            if (int_stack_pop(stack, &l_operand))
            {
                goto rpn_l_operand_not_found_error;
            }

            switch (*pos)
            {

            case '+':
                RPN_STACK_PUSH(l_operand + r_operand);

                break;
            case '-':
                RPN_STACK_PUSH(l_operand - r_operand);

                break;
            case '*':
                RPN_STACK_PUSH(l_operand * r_operand);

                break;
            case '/':
                if (r_operand == 0)
                {
                    goto rpn_zero_div_error;
                }

                RPN_STACK_PUSH(l_operand / r_operand);

                break;
            case '%':
                if (r_operand == 0)
                {
                    goto rpn_zero_div_error;
                }

                RPN_STACK_PUSH(l_operand % r_operand);

                break;
            default:
                goto rpn_unknown_operator_error;
            }
        }
        else if (isspace(*pos)) // space
        {
            space_count++;
        }
        else // invalid char
        {
            goto rpn_invalid_char_error;
        }
        pos++;
    }

    if (int_stack_get_size(stack) != 1)
    {
        goto rpn_too_few_operators_error;
    }

    int_stack_pop(stack, calc_res);

    int_stack_free(stack);

    return 0;

rpn_r_operand_not_found_error:
    fprintf(stderr, "rpn() right operand is not found\n");
    goto rpn_error_exit;

rpn_l_operand_not_found_error:
    fprintf(stderr, "rpn() left operand is not found (right: %d)\n", r_operand);
    goto rpn_error_exit;

rpn_zero_div_error:
    fprintf(stderr, "rpn() zero division (left: %d, right: %d)\n", l_operand, r_operand);
    goto rpn_error_exit;

rpn_unknown_operator_error:
    fprintf(stderr, "rpn() unknown operator (left: %d, right: %d)\n", l_operand, r_operand);
    goto rpn_error_exit;

rpn_invalid_char_error:
    fprintf(stderr, "rpn() invalid character\n");
    goto rpn_error_exit;

rpn_too_few_operators_error:
    fprintf(stderr, "rpn() too few operators\n");
    goto rpn_error_exit;

rpn_memory_error:
    fprintf(stderr, "rpn() memory error\n");
    goto rpn_error_exit;

rpn_error_exit:
    fprintf(stderr, "%s\n", string);
    fprintf(stderr, "%*s(%ld)\n", (int)(pos - string + 1), "^", pos - string + 1);
    fprintf(stderr, "stack contents:\n");
    int_stack_dump_stderr(stack);

    int_stack_free(stack);

    return 1;

rpn_stack_alloc_error:
    fprintf(stderr, "rpn() int_stack_new() error\n");

    return 1;
}

int main(void)
{
    char *string;
    int calc_res;

    while (1)
    {
        string = getstring();
        if (string == NULL)
        {
            return 1;
        }

        if (strcmp(string, "0") == 0)
        {
            free(string);
            return 0;
        }

        if (!rpn(string, &calc_res))
        {
            printf("%d\n", calc_res);
        }

        free(string);
    }
}
