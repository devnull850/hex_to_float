
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DOUBLE_PRECISION_MODE 1
#define SINGLE_PRECISION_MODE 0

struct ieee_754_bits {
    const int sign_bit;
    const int exponent_msb;
    const int exponent_lsb;
    const int fraction_msb;
    const int bias;
};

const struct ieee_754_bits
DOUBLE_PRECISION_BITS =
    {
        .sign_bit = 63,
        .exponent_msb = 62, 
        .exponent_lsb = 52, 
        .fraction_msb = 51,
        .bias = 1023
    };

const struct ieee_754_bits
SINGLE_PRECISION_BITS =
    { 
        .sign_bit = 31, 
        .exponent_msb = 30, 
        .exponent_lsb = 23, 
        .fraction_msb = 22,
        .bias = 127
    };

double convert_hex_bits_to_double(uint64_t, struct ieee_754_bits);
int is_single_precision_mode(char *);

int main(int argc, char *argv[]) {
    uint64_t n;
    int mode = DOUBLE_PRECISION_MODE; // default to 64 bit (double precision)

    // prompt user for number
    scanf("%lx", &n);

    if (argc > 1 && is_single_precision_mode(argv[1])) {
        mode = SINGLE_PRECISION_MODE; // 32 bit (single precision)
    }

    printf("0x%lx == %lf\n", n,
            convert_hex_bits_to_double(n, mode ? DOUBLE_PRECISION_BITS : SINGLE_PRECISION_BITS));

    return EXIT_SUCCESS;
}

double convert_hex_bits_to_double(uint64_t n, struct ieee_754_bits bits) {
    double result, base, divisor;
    int exp, p;

    // 64 bit/double precision: bits 0-51 (52 bits) -> fraction
    // 32 bit/single precision: bits 0-22 (23 bits) -> fraction
    base = 1.0f;
    divisor = 0.5f; // 2 ^ -1 = 0.5

    for (int i = bits.fraction_msb; i >= 0; --i) {
        if ((n >> i) & 1) { base += divisor; }
        divisor /= 2.0f;
    }

    // 64 bit/double precision: bits 52-62 (11 bits) -> exponent
    // 32 bit/single precision: bits 23-30 ( 8 bits) -> exponent
    p = 0;

    for (int i = bits.exponent_lsb, e = 1; i < bits.exponent_msb + 1; ++i, e *= 2) {
        if ((n >> i) & 1) { p += e; }
    }

    // 64 bit/double precision: bias -1023
    // 32 bit/single precision: bias -127
    p -= bits.bias;
    exp = 1;

    if (p > 0) {
        exp <<= p;
    }
    else if (p < 0) {
        p *= -1;
        exp >>= p;
    }

    // 64 bit/double precision: bit 63 -> sign bit
    // 32 bit/single precision: bit 31 -> sign bit
    result = base * exp;
    if ((n >> bits.sign_bit) & 1) { result *= -1; }

    return result;
}

int is_single_precision_mode(char *arg) {
    int flag = 0;
    size_t len;
    char *s;

    len = strlen(arg);

    if ((s = (char *) malloc(len + 1)) == NULL) {
        puts("malloc failed");
        exit(EXIT_FAILURE);
    }

    strncpy(s, arg, len);

    flag = !strncmp(s, "-m32", len);

    if (!s) {
        puts("invalid pointer state");
        exit(EXIT_FAILURE);
    }

    free(s);

    return flag;
}
