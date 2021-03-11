#include<stdio.h>

int main () {
    int a[3] = {1, 2, 3};
    
    unsigned i = 0;
    // printf("i의 크기 : %lu\n", sizeof(i));

    // unsigned long j = sizeof(a)/sizeof(int);
    // printf("j의 크기 : %lu\n", sizeof(j));

    // i = 0;
    // i--;
    // printf("%d, %u\n", i, i);

    for (i = sizeof(a)/sizeof(int) - 1; i >= 0; i--) {
        printf("%d, %u\n", i, i);
    }
    // 해당 코드의 출력은 무한루프된다.
    // i가 unsigned 이기 때문에 만약 signed 였을 시 음수였을 값이라도 양수로 표현되기 때문에 for문이 멈추지 못하기 때문이다.
    return 0;
}