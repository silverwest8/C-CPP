#include <stdio.h>
#include <math.h>

#define MAX_KEY_LEN 10
#define MAX_TEXT_LEN 5000

int len;
int keyLen;
int keyStreamLen;
int frequency[256];
unsigned char ciphertxt[MAX_TEXT_LEN];
unsigned char plaintxt[MAX_TEXT_LEN];
unsigned char key[MAX_KEY_LEN];
unsigned char keyStream[MAX_TEXT_LEN];
double Sums[MAX_KEY_LEN];
double sums[256];

void checkFreq_forKeyLen(int i) {
    keyStreamLen = 0;
    for(int j = 0; j < 256; j++) { frequency[j] = 0; }
    for (int j = 0; j < len; j+=i) {
        frequency[ciphertxt[j]]++;
        keyStreamLen++;
    }
}
double getSumOfq2_forKeyLen() {
    double sumOfq2 = 0;
    for(int j = 0; j < 256; j++) {
        sumOfq2 += pow(((double)frequency[j]/keyStreamLen), 2);
    }
    return sumOfq2;
}
int getMaxKeyLen() {
    int Max = 0; 
    for (int j = 0; j < 10; j++) {
        if (Sums[j] > Sums[Max]) { Max = j; }
    }
    return Max+1;
}
void XORatKeyStream (int i, unsigned int j) {
    keyStreamLen = 0;
    for (int l = i; l < len; l+=keyLen) {
        keyStream[keyStreamLen] = ciphertxt[l] ^ j;
        keyStreamLen++;
    }
}
void checkFreq_forKeyVal() {
    for (int l = 0; l < 256; l++) {
        frequency[l] = 0;
    }
    for (int l = 0; l < keyStreamLen; l++) {
        frequency[keyStream[l]]++;
    }
}
double getSumOfq2_forKeyVal() {
    double sumOfq2 = 0;
    for(int l = 97; l < 123; l++) {
        sumOfq2 += pow(((double)frequency[l]/keyStreamLen), 2);
    }
    return sumOfq2;
}



int getKeyValue() {
    int max = 0;
    for (int j = 0; j < 256; j++) {
        if ( sums[j] > sums[max] ) { max = j; }
    }
    return max;
}
void checkRepetition() {
    for (int i = 1; i <= keyLen; i++) {
        if ( keyLen % i == 0) {
            int j = 0;
            while (j < keyLen) {
                if (key[j] == key[j+i]) {
                    j = j+i;
                    if (j == keyLen-j) {
                        keyLen = i;
                        return;
                    }
                }
                else {
                    break;
                }
            }
        }
    }
}
void decryptAtPlaintxt(void) {
    for (int i = 0; i < len; i++) {
        plaintxt[i] = ciphertxt[i] ^ key[i % keyLen];
    }
}

int main() {
    //File Reading
    unsigned char ch;
    FILE *fpIn, *fpOut;
    fpIn = fopen("hw1_input.txt", "r");
    fpOut = fopen("hw1_output.txt", "w");
    for (int i = 0; fscanf(fpIn, "%c", &ch) != EOF; ++i) {
        ciphertxt[i] = ch;
        len++;
    }

    //to figure out KEY LEN
    for (int i = 1; i <= MAX_KEY_LEN; i++) {
        checkFreq_forKeyLen(i);
        Sums[i-1] = getSumOfq2_forKeyLen();
    }
    keyLen = getMaxKeyLen();

    //to figure out KEY VALUE
    for (int i = 0; i < keyLen; i++) {
        for (unsigned int j = 0x00; j <= 0xff; j++) {
            XORatKeyStream(i, j);
            checkFreq_forKeyVal();
            sums[j] = getSumOfq2_forKeyVal();
        }
        key[i] = getKeyValue();
    }
    checkRepetition();
    decryptAtPlaintxt();

    //File Wiriting
    for (int i = 0; i < keyLen; i++) {
        fprintf(fpOut, "0x%x ", key[i]);
    }
    fprintf(fpOut, "\n");
    for (int i = 0; i < len; i++) {
        fwrite(&plaintxt[i], sizeof(plaintxt[i]), 1, fpOut);
    }
    fclose ( fpIn );
    fclose ( fpOut );
    return 0;
}