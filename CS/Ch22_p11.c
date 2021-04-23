#include <stdio.h>
int gcd_algorithm(int x, int y) {
    if (y == 0) return x;
    return gcd_algorithm(y, (x % y));
}

int main(void) {
    int num1, num2, gcd;
    printf("\nEnter two numbers: ");
    scanf("%d%d", &num1, &num2);
    if (num1<0) num1 = -num1;
    if (num2<0) num2 = -num2;
    gcd = gcd_algorithm(num1, num2);
    if (gcd)
        printf("\nThe GCD of %d and %d is %d\n", num1, num2, gcd);
    else
        printf("\nInvalid input!!!\n");
    return 0;
}