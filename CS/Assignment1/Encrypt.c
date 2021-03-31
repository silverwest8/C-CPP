#include <stdio.h>
#define KEY_LENGTH 3 // Can be anything from 1 to 10

int main() {
    unsigned char ch ;
    FILE *fpIn, *fpOut;
    unsigned char key [KEY_LENGTH] = { 0x31, 0xaa, 0xff};
    /* key values can be changed */

    fpIn = fopen("plaintext.txt", "r");
    fpOut = fopen("hw1_input.txt", "wb");

    for (int i = 0; fscanf(fpIn, "%c", &ch) != EOF; ++i) {
        ch ^= key[i % KEY_LENGTH];  //ch = ch ^ key[i % KEY_LENGTH];
        fwrite(&ch, sizeof(ch), 1, fpOut);
    }
    fclose ( fpIn );
    fclose ( fpOut ) ;
    return 0 ;
}