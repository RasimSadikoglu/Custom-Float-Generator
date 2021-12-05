#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "binary_number.h"

binary_number** get_file_content(char *file_path);
void command_line_interface();

#endif // IO_HANDLER_H