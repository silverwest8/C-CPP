#include <stdio.h>
int main() {
    char *sh[2];
    sh[0]="/bin/sh";
    sh[1]=NULL;
    setuid(0, 0);
    execve(sh[0], sh, 0);
    return 0;
}