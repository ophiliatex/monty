#include "monty.h"
monty_t  global_data = {NULL, NULL, NULL, NULL, NULL, 0};

int main(int argc, char **argv) {
    size_t len = 0;

    FILE *file;

    if (argc != 2) {
        fprintf(stderr, "USAGE: monty file\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    /* TODO: Read and process file line by line. */

    init_monty(argv[1]);


    while (_getline(&(global_data.line), &len, global_data.file) != (size_t)-1) {
        global_data.line_number++;
        global_data.opcode = strtok(global_data.line, " \t\n\r");
        if (global_data.opcode == NULL || global_data.opcode[0] == '$')
            continue;

        global_data.argument = strtok(NULL, " \t\n\r");

        execute_opcode();
    }

    cleanup_monty();

    fclose(file);
    return (0);
}
