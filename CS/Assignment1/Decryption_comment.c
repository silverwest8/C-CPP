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

void chekFreq_forKeyLen(int i) {
    keyStreamLen = 0;
    for(int j= 0; j < 256; j++) { frequency[j] = 0; }
    for (int j=0; j < len; j+=i) {
        frequency[ciphertxt[j]]++;
        keyStreamLen++;
    }
}
void chekFreq_forKeyVal() {
    for (int l = 0; l < 256; l++) {
        frequency[l] = 0;
    }
    for (int l = 0; l < keyStreamLen; l++) {
        frequency[keyStream[l]]++;
    }
}
void XORatKeyStream (int i, unsigned int j) {
    keyStreamLen = 0;
    for (int l = i; l < len; l+=keyLen) {
        keyStream[keyStreamLen] = ciphertxt[l] ^ j;
        // 각 스트림에 대해서 256개의 키를 가지고 XOR을 수행
        // printf("%c", keyStream[a]);
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
    //계산한 q^2의 합들 중에서 가장 큰 것의 인덱스, Max+1이 유력한 키 길이가 되지만,
    //그것의 약수나 배수가 키값일 수 있음
    for (int j = 0; j < 10; j++) {
        if (Sums[j] > Sums[Max]) { Max = j; }
    }

    return Max+1;
}
double getSumOfq2_forKeyVal() {
    double sumOfq2 = 0;
    for(int l = 97; l < 123; l++) {
        if (frequency[l] != 0) {
            sumOfq2 += pow(((double)frequency[l]/keyStreamLen), 2);
        }
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
                    // printf("%#x, %d\n", key[j], i);
                } else {
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
    unsigned char ch;

    FILE *fpIn, *fpOut;
    fpIn = fopen("hw1_input.txt", "r");
    fpOut = fopen("hw1_output.txt", "w");

    //cipher에 input 복사, len 구하기
    for (int i = 0; fscanf(fpIn, "%c", &ch) != EOF; ++i) {
        ciphertxt[i] = ch;
        len++;
    }

    for (int i = 1; i <= MAX_KEY_LEN; i++){
        //frequency 0 초기화, 값세팅, keyStreamLen(키스트림 길이)
        chekFreq_forKeyLen(i);

        //sum of distribution
        //값 비교를 위해 저장
        Sums[i-1] = getSumOfq2_forKeyLen();
    }
    //키 길이 결정
    keyLen = getMaxKeyLen();
    // 키의 내용을 찾기
    for (int i = 0; i < keyLen; i++) {
        //max키길이의 약수가 key 길이라고 가정
        for (unsigned int j = 0x00; j <= 0xff; j++) {
            XORatKeyStream(i, j);
            chekFreq_forKeyVal();
            // 어떤 값을 키값으로 가정했을 때 각 소문자의 빈도의 비율의 제곱의 합을 계산하고 sums[j]에 저장
            sums[j] = getSumOfq2_forKeyVal();
        }
        //0x00부터 0xff로 복호화해서 각 소문자의 빈도의 비율의 제곱의 합 중 최대값을 키값으로 지정
        key[i] = getKeyValue();
    }

    // 키값 내에서 반복이 있는지 확인하기
    checkRepetition();

    //plaintxt 배열에 복호화
    decryptAtPlaintxt();

    //파일에 쓰기
    for (int i = 0; i < keyLen; i++) { fprintf(fpOut, "0x%x ", key[i]); }
    fprintf(fpOut, "\n");
    for (int i = 0; i < len; i++) { fwrite(&plaintxt[i], sizeof(plaintxt[i]), 1, fpOut); }

    //파일 입출력 스트림 닫기
    fclose ( fpIn );
    fclose ( fpOut ) ;
    return 0 ;
}