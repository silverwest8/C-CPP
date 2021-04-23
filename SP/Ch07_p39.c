#include <stdio.h>

typedef union {
    float f;
    unsigned u;
} bit_float_t;

float bit2float(unsigned u) {
    bit_float_t arg;
    arg.u = u;
    return arg.f;
}

unsigned float2bit(float f) {
    bit_float_t arg;
    arg.f = f;
    return arg.u;
}

int main() {
    unsigned int u = 10;
    float u2f = bit2float(u);
    float f_u = (float)u;

    printf("bit2float(%u) = %f\n", u, bit2float(u));
    printf("is not same as (float)u %f\n", (float)u);

    for (int i = 31; i >= 0; i--) {
        printf("%d", (u >> i) & 1 );
    }
    printf("\n");
    unsigned u2fB = *(unsigned *)&u2f;
    for (int i = 31; i >= 0; i--) {
        printf("%d", (u2fB >> i) & 1 );
    }
    printf("\n");

    unsigned f_uB = *(unsigned *)&f_u;
    for (int i = 31; i >= 0; i--) {
        printf("%d", (f_uB >> i) & 1 );
    }
    printf("\n");printf("\n");
    /*
    The result of bit2float(u) is not same as (float)u
    In union, the elements share the same memory, so the result of bit2float have same bit pattern with input value, but have different value.
    However, by casting, the value is kept but the bit pattern change.
    */

    float f = 10;
    unsigned f2u = float2bit(f);
    unsigned u_f = (unsigned)f;

    printf("float2bit(%f) = %u\n", f, f2u);
    printf("is not same as (unsigned)f %u\n", u_f);

    unsigned fB = *(unsigned *)&f;
    for (int i = 31; i >= 0; i--) {
        printf("%d", (fB >> i) & 1 );
    }
    printf("\n");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (f2u >> i) & 1 );
    }
    printf("\n");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (u_f >> i) & 1 );
    }
    /*
    The result of float2bit(f) is not same as (unsigned)f
    In union, the elements share the same memory, so the result of float2bit have same bit pattern with input value, but have different value.
    However, by casting, the value is kept but the bit pattern change.
    */
    return 0;
}