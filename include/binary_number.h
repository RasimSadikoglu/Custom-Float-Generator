#ifndef BINARY_NUMBER_H
#define BINARY_NUMBER_H

typedef long long i64;
typedef unsigned char u8;

enum type {
    DECIMAL,
    FLOAT
};

typedef struct {
    int type;
    union {
        i64 d;
        double f;
    } number;
} binary_number;

#endif // BINARY_NUMBER_H