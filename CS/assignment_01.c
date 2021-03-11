#include <stdio.h>
#define KEY_LENGTH 3 // Can be anything from 1 to 10

int main() {
    unsigned char ch ;
    FILE *fpIn, *fpOut;
    unsigned char key [KEY_LENGTH] = { 0x01 , 0x02 , 0x03 };
    /* key values can be changed */
    fpIn = fopen("plaintext.txt", "r");
    fpOut = fopen("hw1 input.txt", "wb");
    for (int i = 0; fscanf(fpIn, "%c", &ch) != EOF; ++i) {
        ch ^= key[i % KEY_LENGTH]; fwrite(&ch, sizeof(ch), 1, fpOut);
    }
    fclose ( fpIn );
    fclose ( fpOut ) ;
    return 0 ;
}