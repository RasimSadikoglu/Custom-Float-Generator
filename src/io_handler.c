#include "../include/io_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 50

binary_number** get_file_content(char *file_path) {

    FILE *input = fopen(file_path, "r");

    if (input == NULL) {
        perror("File does not exist!");
        exit(EXIT_FAILURE);
    }

    binary_number **bn = calloc(sizeof(binary_number*), 1);
    int current_index = 0;

    char *buffer = malloc(STRING_SIZE);

    while (fgets(buffer, STRING_SIZE, input) != NULL) {
        
        bn[current_index] = malloc(sizeof(binary_number));

        int type = bn[current_index]->type = (strchr(buffer, '.') != NULL);

        if (type == DECIMAL) bn[current_index]->number.d = strtoll(buffer, NULL, 10);
        else if (type == FLOAT) bn[current_index]->number.f = atof(buffer);

        current_index++;

        bn = realloc(bn, sizeof(binary_number*) * (current_index + 1));
        bn[current_index] = NULL;
    }

    free(buffer);
    fclose(input);

    return bn;
}

void command_line_interface() {

}