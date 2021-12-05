#include <stdio.h>

#include "../include/io_handler.h"
#include "../include/converter.h"

int main(int argc, char **argv) {

    if (sizeof(double) != 8) exit(EXIT_FAILURE);
    
    binary_number **bns = get_file_content(argv[1]);

    for (binary_number **it = bns; *it != NULL; it++) {

        if ((*it)->type == DECIMAL) {
            printf("%-15lld -> 0x%X\n", (*it)->number.d, decimal_to_hex(*it, 5));
        } else {
            printf("%-15g -> 0x%X\n", (*it)->number.f, float_to_hex(*it, 8, 7));
        }

    }
    
}