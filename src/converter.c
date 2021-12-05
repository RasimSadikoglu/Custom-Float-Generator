#include "../include/converter.h"

#include <string.h>

#define D_EXP 11
#define D_MAN 52

i64 get_shape(i64 decimal_size) {

    i64 shape = 0;
    
    for (; decimal_size > 0; decimal_size--) {
        shape <<= 1;
        shape++;
    }

    return shape;
}

i64 decimal_to_hex(i64 number, size_t decimal_size) {

    return number & get_shape(decimal_size);
}

i64 float_to_hex(double number, size_t exponent_size, size_t mantissa_size) { 

    if (exponent_size + mantissa_size > 63 || number == 0) return 0;

    i64 num;
    memcpy(&num, &number, 8);

    i64 mantissa = num & get_shape(D_MAN);
    i64 exponent = (num >> D_MAN) & get_shape(D_EXP);
    i64 sign = (num >> (D_EXP + D_MAN)) & 1;

    /* Mantissa */
    i64 round_size = D_MAN - mantissa_size;

    if (round_size > 0) {
        i64 round_part = mantissa & get_shape(round_size);
        mantissa = mantissa >> (round_size);

        i64 round_shape = get_shape(round_size) ^ get_shape(round_size - 1);

        if (round_part == round_shape && mantissa & 1) mantissa++;

        if (round_part > round_shape) mantissa++;

        if (mantissa & (get_shape(mantissa_size + 1) ^ get_shape(mantissa_size))) {
            exponent++;
            mantissa >>= 1;
        }
    }
    /* Mantissa */

    /* Exponent */
    exponent += get_shape(exponent_size - 1) - get_shape(D_EXP - 1);

    if (exponent >= get_shape(exponent_size)) {
        mantissa = 0;
        exponent = get_shape(exponent_size);
    }

    if (exponent <= 0) {
        mantissa |= 1 << mantissa_size;
        mantissa >>= exponent * -1 + 1;
        exponent = 0;
    }
    /* Exponent */

    return (sign << (exponent_size + mantissa_size)) | (exponent << mantissa_size) | mantissa;
}