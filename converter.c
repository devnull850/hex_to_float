
#include <stdint.h>
#include <stdio.h>

double convert_hex_bits_to_double(uint64_t, int, int, int, int);

int main(void) {
    uint64_t n;

    // prompt user for number
    scanf("%lx", &n);

    printf("0x%lx == %lf\n", n,
            convert_hex_bits_to_double(n, 51, 62, 52, 63));

    return 0;
}

double convert_hex_bits_to_double(uint64_t n,
        int fraction_msb, int exp_msb, int exp_lsb, int sign_bit) {
    double result, base, divisor;
    int exp, p;

    // bits 0-51 (52 bits) -> fraction
    base = 1.0f;
    divisor = 0.5f; // 2 ^ -1 = 0.5

    for (int i = fraction_msb; i >= 0; --i) {
        if ((n >> i) & 1) { base += divisor; }
        divisor /= 2.0f;
    }

    // bits 52-62 (11 bits) -> exponent
    p = 0;

    for (int i = exp_lsb, e = 1; i < exp_msb + 1; ++i, e *= 2) {
        if ((n >> i) & 1) { p += e; }
    }

    p -= 1023;
    exp = 1;

    if (p > 0) {
        exp <<= p;
    }
    else if (p < 0) {
        p *= -1;
        exp >>= p;
    }

    // bit 63 -> sign bit
    result = base * exp;
    if ((n >> sign_bit) & 1) { result *= -1; }

    return result;
}
