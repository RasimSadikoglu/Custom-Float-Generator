#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORDSIZE 50
#define HEXSIZE 8

typedef struct {
	char **arr;
	unsigned size;
} string_arr;

void string_arr_free(string_arr arr) {
	unsigned i; for (i = 0; i < arr.size; i++) free(arr.arr[i]);
	free(arr.arr);
	return;
}

void print_menu();

string_arr read_file(char*);
void print_file(string_arr);

string_arr convert_numbers(string_arr);

char* parse_float(char*);
char* parse_unsigned(char*);
char* parse_int(char*);

char* binary_to_hex(unsigned, unsigned);

char digit_to_hex(unsigned n) {return n < 10 ? n + 48 : n + 55;}
int is_digit(char ch) {return ch >= '0' && ch <= '9';}
int power(int b, int p) {return p == 1 ? b : b * power(b, p - 1);}

const unsigned float_exp_size[4] = {3, 8, 10, 12};
const unsigned float_fraction_size[4] = {4, 7, 12, 12};
unsigned float_size;
unsigned byte_ordering;

int main() {

	print_menu();

	string_arr string_numbers = read_file("input.txt");

	string_arr hex_numbers = convert_numbers(string_numbers);

	print_file(hex_numbers);

	string_arr_free(string_numbers);
	string_arr_free(hex_numbers);

	return 0;
}

void print_menu() {

	printf("1.Big Endian\n2.Little Endian\n");
	do {
		printf("Byte Ordering: ");
		scanf("%u", &byte_ordering);
	} while (byte_ordering != 1 && byte_ordering != 2);

	do {
		printf("Float Size (in bytes) (1 - 4): ");
		scanf("%u", &float_size);
	} while (float_size < 1 || float_size > 4);

	return;
}

string_arr read_file(char* target) {

	FILE *target_file = fopen(target, "r");

	string_arr string_numbers = {};
	string_numbers.size = 1;
	string_numbers.arr = calloc(string_numbers.size, sizeof(char*));
	string_numbers.arr[0] = calloc(WORDSIZE, sizeof(char));

	char ch = getc(target_file);
	unsigned i; for (i = 0; ch != EOF; i++) {
		if (string_numbers.size < i + 1) {
			string_numbers.size++;
			string_numbers.arr = realloc(string_numbers.arr, string_numbers.size * sizeof(char*));
			string_numbers.arr[string_numbers.size - 1] = calloc(WORDSIZE, sizeof(char));
		}

		int j; for (j = 0; ch != '\n' && ch != EOF; j++) {
			if (j == WORDSIZE) {
				printf("WORDSIZE is not big enough for element %d!\nPlease increase the WORDSIZE and try again!", i);
				exit(1);
			}
			while (ch == ' ') ch = getc(target_file);
			if (ch == 'U') ch = 'u';
			string_numbers.arr[i][j] = ch;
			ch = getc(target_file);
		}
		string_numbers.arr[i][j] = '\0';

		do {ch = getc(target_file);} while (ch == ' ');
	}

	fclose(target_file);

	return string_numbers;
}

void print_file(string_arr hex_numbers) {

	FILE *target_file = fopen("output.txt", "w");

	unsigned i; for (i = 0; i < hex_numbers.size; i++) {
		if (i) fprintf(target_file, "\n");

		char *hex = hex_numbers.arr[i];

		unsigned j;
		if (byte_ordering - 1) for (j = HEXSIZE - 1; j < HEXSIZE; j -= 2) {
			if (hex[j] == 0) continue;

			fprintf(target_file, "%c%c ", hex[j - 1], hex[j]);
		}

		else for (j = 0; j < HEXSIZE; j += 2) {
			if (hex[j] == 0) continue;

			fprintf(target_file, "%c%c ", hex[j], hex[j + 1]);
		}
	}

	fclose(target_file);

	printf("\n\nOutput.txt is generated!\n");
	return;
}

string_arr convert_numbers(string_arr string_numbers) {

	string_arr hex_numbers;
	hex_numbers.size = string_numbers.size;
	hex_numbers.arr = malloc(hex_numbers.size * sizeof(char*));

	unsigned i; for (i = 0; i < hex_numbers.size; i++) {
		if (strchr(string_numbers.arr[i], '.') != NULL) hex_numbers.arr[i] = parse_float(string_numbers.arr[i]);
		else if (strchr(string_numbers.arr[i], 'u') != NULL) hex_numbers.arr[i] = parse_unsigned(string_numbers.arr[i]);
		else hex_numbers.arr[i] = parse_int(string_numbers.arr[i]);
	}

	return hex_numbers;
}

char* parse_float(char* str) {

	unsigned sign = str[0] == '-' ? 1 : 0;

	float number = 0;

	unsigned i; for (i = sign; str[i] != '.'; i++) number = number * 10 + str[i] - '0';
	i++;

	float mult = 1;
	for (; i < WORDSIZE && str[i] != '\0' && is_digit(str[i]); i++) {
		mult /= 10;
		number += (str[i] - '0') * mult;
	}

	unsigned bias = power(2, float_exp_size[float_size - 1] - 1) - 1;

	unsigned fraction;
	int exponent;

	memcpy(&fraction, &number, sizeof(float));

	exponent = (fraction & 0x7F800000) >> 23;
	fraction = fraction & 0x007FFFFF;

	unsigned round_part = fraction << (9 + float_fraction_size[float_size - 1]);
	fraction = fraction >> (23 - float_fraction_size[float_size - 1]);

	if (round_part > 0x80000000 || (round_part == 0x80000000 && fraction & 0x1)) {
		fraction++;
		if (fraction & (1 << float_fraction_size[float_size - 1])) {
			exponent++;
			fraction = fraction >> 1;
		}
	}

	exponent += bias - 127;

	if (exponent <= 0) {
		fraction = fraction | (1 << float_fraction_size[float_size - 1]);
		fraction = fraction >> (exponent * -1 + 1);
		exponent = 0;
	} else if (exponent > power(2, float_exp_size[float_size - 1]) - 2) {
		fraction = 0;
		exponent = power(2, float_exp_size[float_size - 1]) - 1;
	}

	if (float_size == 3) fraction = fraction << 1;
	if (float_size == 4) fraction = fraction << 7;

	exponent = exponent << (float_size * 8 - 1 - float_exp_size[float_size - 1]);
	sign = sign << (float_size * 8 - 1);
	unsigned final_bits = sign | exponent | fraction;

	if (number == 0) final_bits = 0;

	return binary_to_hex(final_bits, float_size);
}

char* parse_unsigned(char* str) {

	int i; for (i = 0; str[i] != 'u' && str[i] != 'U'; i++);
	while (i >= 0 && !is_digit(str[i])) i--;

	unsigned mult = 1, number = 0;
	while (mult < 1000000 && i >= 0 && is_digit(str[i])) {
		number += (str[i--] - '0') * mult;
		mult *= 10;
	}

	return binary_to_hex(number, 2);
}

char* parse_int(char* str) {

	int i; for (i = 0; str[i] != '\0'; i++);
	while (i >= 0 && !is_digit(str[i])) i--;

	int sign = str[0] == '-' ? 1 : 0;

	int mult = 1, number = 0;
	while (mult < 1000000 && i >= sign && is_digit(str[i])) {
		number += (str[i--] - '0') * mult;
		mult *= 10;
	}

	if (sign) number *= -1;

	return binary_to_hex(number, 2);
}

char* binary_to_hex(unsigned number, unsigned size) {

	char *hex = calloc(HEXSIZE, sizeof(char));

	int i; for (i = size * 2 - 1; i >= 0; i--) {
		hex[i] = digit_to_hex(number & 0xF);
		number >>= 4;
	}

	return hex;
}
