#include "monty.h"

void init_monty(char *filename)
{
    global_data.file = fopen(filename, "r");
    if (global_data.file == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    global_data.stack = NULL;

}

void cleanup_monty(void)
{
    stack_t *current, *tmp;

    if (global_data.file)
        fclose(global_data.file);
    if (global_data.line)
        free(global_data.line);

    current = global_data.stack;
    while (current)
    {
        tmp = current;
        current = current->next;
        free(tmp);
    }
}

void execute_opcode(void)
{
    instruction_t opcodes[] = {
            {"push", push_op},
            {"pall", pall_op},
            {"pint", pint_op},
            {"pop", pop_op},
            {"swap", swap_op},
            {"add", add_op},
            {"nop", nop_op},
            {"sub", sub_op},
            {"div", div_op},
            {"mul", mul_op},
            {"mod", mod_op},
            {"#", nop_op},
            {"", nop_op},
            {NULL, NULL}
    };

    int i = 0;
    while (opcodes[i].opcode)
    {
        if (strcmp(global_data.opcode, opcodes[i].opcode) == 0)
        {
            opcodes[i].f(&(global_data.stack), global_data.line_number);
            return;
        }
        i++;
    }

    fprintf(stderr, "L%d: unknown instruction %s\n",
            global_data.line_number, global_data.opcode);
    exit(EXIT_FAILURE);
}


size_t _getline(char **lineptr, size_t *n, FILE *stream) {
    int c;
    char *temp;
    size_t i = 0;

    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = (char *)malloc(*n);

        if (*lineptr == NULL) {
            return (-1);
        }
    }



    while ((c = fgetc(stream)) != EOF) {
        if (i >= *n - 1) {
            *n *= 2;

            temp = (char *)realloc(*lineptr, *n);

            if (temp == NULL) {
                return (-1);
            }
            *lineptr = temp;
        }

        (*lineptr)[i++] = (char)c;

        if (c == '\n') {
            break;
        }
    }

    if (i == 0 && c == EOF) {
        return (-1);
    }

    (*lineptr)[i] = '\0';

    return (i);
}
