#include <stdio.h>
#include <math.h>

int main () {
    double sum = 0;
    double a = 0;
    sum += pow(0.082, 2);
    sum += pow(0.015, 2);
    sum += pow(0.028, 2);
    sum += pow(0.043, 2);
    sum += pow(0.127, 2);
    sum += pow(0.022, 2);
    sum += pow(0.020, 2);
    sum += pow(0.061, 2);
    sum += pow(0.070, 2);
    sum += pow(0.002, 2);
    sum += pow(0.008, 2);
    sum += pow(0.040, 2);
    sum += pow(0.024, 2);
    sum += pow(0.067, 2);
    sum += pow(0.075, 2);

    sum += pow(0.019, 2);
    sum += pow(0.001, 2);
    sum += pow(0.060, 2);
    sum += pow(0.063, 2);
    sum += pow(0.091, 2);
    sum += pow(0.028, 2);
    sum += pow(0.010, 2);
    sum += pow(0.023, 2);
    sum += pow(0.001, 2);
    sum += pow(0.020, 2);
    sum += pow(0.001, 2);

    a = pow((double)1/256, 2);
    printf("%f\n", sum);
    printf("%f\n", a);
    
    return 0;
}