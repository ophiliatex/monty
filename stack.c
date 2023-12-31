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

/**
* The opcode pop removes the top element of the stack.

Usage: pop
If the stack is empty, print the error message
 L<line_number>: can't pop an empty stack, followed by a new line,
 and exit with the status EXIT_FAILURE
*/

void pop_op(stack_t **stack, unsigned int line_number)
{
    stack_t *current;

    (void) line_number;
    if (*stack == NULL)
    {
        fprintf(stderr, "L%d: can't pop an empty stack\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    current = *stack;
    *stack = (*stack)->next;
    if (*stack)
        (*stack)->prev = NULL;
    free(current);
}

/**
* The opcode swap swaps the top two elements of the stack.
*/

void swap_op(stack_t **stack, unsigned int line_number)
{
    stack_t *current;

    (void) line_number;
    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%d: can't swap, stack too short\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    current = *stack;
    *stack = (*stack)->next;
    current->next = (*stack)->next;
    current->prev = *stack;
    (*stack)->next = current;
    (*stack)->prev = NULL;
}


void add_op(stack_t **stack, unsigned int line_number)
{
    stack_t *current;

    (void) line_number;
    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%d: can't add, stack too short\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    current = *stack;
    *stack = (*stack)->next;
    (*stack)->n += current->n;
    (*stack)->prev = NULL;
    free(current);
}

void nop_op(stack_t **stack, unsigned int line_number)
{
    (void) stack;
    (void) line_number;
}

void sub_op(stack_t **stack, unsigned int line_number)
{
    stack_t *current;

    (void) line_number;
    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%d: can't sub, stack too short\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    current = *stack;
    *stack = (*stack)->next;
    (*stack)->n -= current->n;
    (*stack)->prev = NULL;
    free(current);
}

void div_op(stack_t **stack, unsigned int line_number)
{
    stack_t *current;

    (void) line_number;
    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%d: can't div, stack too short\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    current = *stack;
    if (current->n == 0)
    {
        fprintf(stderr, "L%d: division by zero\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    *stack = (*stack)->next;
    (*stack)->n /= current->n;
    (*stack)->prev = NULL;
    free(current);
}

void mul_op(stack_t **stack, unsigned int line_number)
{
    stack_t *current;

    (void) line_number;
    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%d: can't mul, stack too short\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    current = *stack;
    *stack = (*stack)->next;
    (*stack)->n *= current->n;
    (*stack)->prev = NULL;
    free(current);
}

void mod_op(stack_t **stack, unsigned int line_number)
{
    stack_t *current;

    (void) line_number;
    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%d: can't mod, stack too short\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    current = *stack;
    if (current->n == 0)
    {
        fprintf(stderr, "L%d: division by zero\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    *stack = (*stack)->next;
    (*stack)->n %= current->n;
    (*stack)->prev = NULL;
    free(current);
}

void pchar_op(stack_t **stack, unsigned int line_number)
{
    (void) line_number;
    if (*stack == NULL)
    {
        fprintf(stderr, "L%d: can't pchar, stack empty\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    if ((*stack)->n < 0 || (*stack)->n > 127)
    {
        fprintf(stderr, "L%d: can't pchar, value out of range\n", global_data.line_number);
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "%c\n", (*stack)->n);
    fflush(NULL);
}

void pstr_op(stack_t **stack, unsigned int line_number)
{
    stack_t *current;

    (void) line_number;
    current = *stack;
    while (current)
    {
        if (current->n <= 0 || current->n > 127)
            break;
        fprintf(stdout, "%c", current->n);
        current = current->next;
    }
    fprintf(stdout, "\n");
    fflush(NULL);
}

void rotr_op(stack_t **stack, unsigned int line_number)
{
    stack_t *current, *tmp;

    (void) line_number;
    if (*stack == NULL || (*stack)->next == NULL)
        return;
    current = *stack;
    while (current->next)
        current = current->next;
    tmp = current->prev;
    tmp->next = NULL;
    current->prev = NULL;
    current->next = *stack;
    (*stack)->prev = current;
    *stack = current;
}

void rotl_op(stack_t **stack, unsigned int line_number)
{
    stack_t *current, *tmp;

    (void) line_number;
    if (*stack == NULL || (*stack)->next == NULL)
        return;
    current = *stack;
    tmp = current->next;
    while (current->next)
        current = current->next;
    current->next = *stack;
    (*stack)->prev = current;
    (*stack)->next = NULL;
    *stack = tmp;
    tmp->prev = NULL;
}
