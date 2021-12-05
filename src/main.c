#include <stdio.h>

#include "../include/io_handler.h"
#include "../include/converter.h"

int main(int argc, char **argv) {

    if (sizeof(double) != 8) exit(EXIT_FAILURE);

    command_line_interface();
    
}