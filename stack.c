#include <errno.h>
#include "monty.h"

void push_op(stack_t **stack, unsigned int line_number)
{
    stack_t *new;
    char *endptr;

    (void) line_number;
    new = malloc(sizeof(stack_t));
    if (new == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    errno = 0;    /* To distinguish success/failure after call */
    new->n = (int)strtol(global_data.argument, &endptr, 0);

    /* Check for various possible errors. */

    if (errno != 0 || strcmp(global_data.argument, endptr) == 0 || *endptr != '\0') {
        fprintf(stderr, "L%d: usage: push integer\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }

    new->prev = NULL;
    new->next = *stack;
    if (*stack)
        (*stack)->prev = new;
    *stack = new;
}

void pall_op(stack_t **stack, unsigned int line_number)
{
    stack_t *current;

    (void) line_number;
    current = *stack;
    while (current)
    {
        fprintf(stdout, "%d\n", current->n);
        current = current->next;
    }
    fflush(NULL);
}

void pint_op(stack_t **stack, unsigned int line_number)
{
    (void) line_number;
    if (*stack == NULL)
    {
        fprintf(stderr, "L%d: can't pint, stack empty\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "%d\n", (*stack)->n);
    fflush(NULL);
}
