int gcdExtended(int a, int b, int *x, int *y) {
    // Base Case
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int x1, y1;
    // To store results of recursive call
    int gcd = gcdExtended(b, a%b, &x1, &y1);

    // Update x and y using results of recursive call
    *x = y1;
    *y = x1 - (a/b) * y1;
    return gcd;
}

int main() {
    int x, y;
    int a = 30, b = 21;
    int g = gcdExtended(a, b, &x, &y);
    printf("gcd(%d, %d) = %d, x = %d, y = %d", a, b, g, x, y);
    return 0;
}