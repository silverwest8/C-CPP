#include <stdio.h>

int main(int argc, char * argv[]) {
    int i=0;
    printf("Hello World\n");
    printf("The argc is %d \n", argc);
    for (i=0; i < argc; i++) {
        printf("The %dth element in argv is %s\n", i, argv[i]);
    }
    return(0);
}