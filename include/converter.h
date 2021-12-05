#ifndef CONVERTER_H
#define CONVERTER_H

#include <stdlib.h>

#include "binary_number.h"

i64 decimal_to_hex(binary_number *number, size_t decimal_size);
i64 float_to_hex(binary_number *number, size_t exponent_size, size_t mantissa_size);

i64 hex_to_decimal(char*);
double hex_to_float(char*, size_t exponent_size, size_t mantissa_size);

#endif // CONVERTER_H