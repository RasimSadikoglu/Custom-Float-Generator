# Custom-Float-Generator

This program converts floating point numbers to their IEEE754 representation. It can convert it to any size wanted. Upper limit is bounded to **double**. Therefore it is intended to use with small numbers. 

---
## Usage

You can use either one of the methods to compile and run.
```
make run
```
or
```
mkdir -p bin
gcc src/main.c src/io_handler.c src/converter.c -o bin/main
./bin/main
```
Run this commands from the root directory of this repository.

---
## Commands

> q

Exits from command line interface.

> o

Change float and decimal sizes. Program will prompt necessary information.

> d <[0, 64]>

Change the decimal size.

```
>>> d 10 // this will set decimal size to 10 bits.
```

> e <[0, 63]>

Usage same as 'd'. This will set exponent size for floating point numbers.

> m <[0, 63]>

Usage same as 'd'. This will set mantissa size for floating point numbers.

**Important Note:** Setting float size to a number bigger than 64 will not **produce** correct answers. *(floatsize = 1(sign bit) + exponent size + mantissa size)*

> ff <[0, 1]>

Force float option. When this is set to the 0(false), any number that doesn't contain '.' will be converted to integer.

> f <input.txt> <output.txt>

This will read from the input file and output the result to stdout or to the output file if it is given.

**Numbers has to be seperated with new line, otherwise only the first number on that line will be converted.**
