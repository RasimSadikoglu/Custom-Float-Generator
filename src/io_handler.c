#include "../include/io_handler.h"

#include <stdio.h>
#include <string.h>

#define STRING_SIZE 100

enum bool {false, true};

size_t sizes[] = {64, 11, 52};

void convert_file(char *input_path, char *output_path) {

    FILE *input = fopen(input_path, "r");
    FILE *output = output_path == NULL ? stdout : fopen(output_path, "a");

    if (input == NULL) {
        perror("File does not exist!");
        exit(EXIT_FAILURE);
    }

    char buffer[STRING_SIZE];

    while (fgets(buffer, STRING_SIZE, input) != NULL) {
        if (strchr(buffer, '.') == NULL) {
            i64 number = strtoll(buffer, NULL, 10);
            fprintf(output, "%-15lld -> 0x%X\n", number, decimal_to_hex(number, sizes[0]));
        } else {
            double number = atof(buffer);
            fprintf(output, "%-15G -> 0x%X\n", number, float_to_hex(number, sizes[1], sizes[2]));
        }
    }

    fclose(input);
    if (output_path != NULL) {
        printf("Results have been added to %s.\n", output_path);
        fclose(output);
    }
}

int command_parser(char *buffer) {
    switch (buffer[0]) {
        case 'q':
            exit(EXIT_SUCCESS);
        case 'o':
            printf("Decimal Size (in bits): ");
            scanf("%zu", sizes);
            printf("Exponent Size (in bits): ");
            scanf("%zu", sizes + 1);
            printf("Mantissa Size (in bits): ");
            scanf("%zu", sizes + 2);
            getc(stdin);
            break;
        case 'd':
            sizes[0] = strtoull(buffer + 2, NULL, 10);
            printf("Decimal size is set to %zu.\n", sizes[0]);
            break;
        case 'e':
            sizes[1] = strtoull(buffer + 2, NULL, 10);
            printf("Exponent size is set to %zu.\n", sizes[1]);
            break;
        case 'm':
            sizes[2] = strtoull(buffer + 2, NULL, 10);
            printf("Mantissa size is set to %zu.\n", sizes[2]);
            break;
        case 'f':
            char *input_path = strtok_r(buffer + 2, " \n", &buffer);
            char *output_path = strtok_r(buffer, " \n", &buffer);
            convert_file(input_path, output_path);
            break;
        case 'h':
            printf("Please check the README file. This section will be written in the future.\n");
            break;
        default:
            return false;
    }

    return true;
}

void command_line_interface() {

    for (;;) {

        printf(">>> ");

        char buffer[STRING_SIZE];
        fgets(buffer, STRING_SIZE, stdin);

        if (command_parser(buffer)) continue;

        if (strchr(buffer, '.') == NULL) {
            i64 number = strtoll(buffer, NULL, 10);
            printf("%-15lld -> 0x%X\n", number, decimal_to_hex(number, sizes[0]));
        } else {
            double number = atof(buffer);
            printf("%-15G -> 0x%X\n", number, float_to_hex(number, sizes[1], sizes[2]));
        }

    }

}