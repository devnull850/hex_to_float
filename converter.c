
#include <stdint.h>
#include <stdio.h>

int main(void) {
    uint64_t n;
    double result, base, divisor;
    int exp, p;

    // prompt user for number
    scanf("%lx", &n);

    // bits 0-51 (52 bits) -> fraction
    base = 1.0f;
    divisor = 0.5f; // 2 ^ -1 = 0.5

    for (int i = 51; i >= 0; --i) {
        if ((n >> i) & 1) { base += divisor; }
        divisor /= 2.0f;
    }

    // bits 52-62 (11 bits) -> exponent
    p = 0;

    for (int i = 52, e = 1; i < 63; ++i, e *= 2) {
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
    if ((n >> 63) & 1) { result *= -1; }

    printf("0x%lx == %lf\n", n, result);

    return 0;
}
