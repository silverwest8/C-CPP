#include <stdio.h>
#include <math.h>

#define MAX_KEY_LEN 10
#define MAX_TEXT_LEN 5000

void dycrypt(unsigned char ciphertxt[], int len, unsigned char key[], int keyLen, unsigned char plaintxt[]) {
    // printf("\nkeyLen : %d\n", keyLen);
    for (int i = 0; i < keyLen; i++) { printf("%#x ", key[i]); }
    printf("\n");
    for (int i = 0; i < len; i++) {
        plaintxt[i] = ciphertxt[i] ^ key[i % keyLen];
        printf("%c", plaintxt[i]);
    }
}

int main() {
    unsigned char ch;
    unsigned char ciphertxt[MAX_TEXT_LEN];
    unsigned char plaintxt[MAX_TEXT_LEN];
    int len = 0;
    int frequency[256];

    FILE *fpIn, *fpOut;
    fpIn = fopen("hw1_input.txt", "r");
    fpOut = fopen("hw1_output.txt", "w");

    //cipher에 input 복사, len 구하기
    for (int i = 0; fscanf(fpIn, "%c", &ch) != EOF; ++i) {
        ciphertxt[i] = ch;
        len++;
    }
    // printf("len : %d!\n", len);

    int keyStreamLen;
    double sumOfq2;
    double Sums[MAX_KEY_LEN];
    for (int i = 1; i <= MAX_KEY_LEN; i++){
        // printf("\nKey 길이가 %d인 경우\n", i);

        //frequency 0 초기화, 값세팅, keyStreamLen(키스트림 길이)
        for(int j= 0; j < 256; j++) { frequency[j] = 0; }

        keyStreamLen = 0;
        for (int j=0; j < len; j+=i) {
            frequency[ciphertxt[j]]++;
            keyStreamLen++;
        }
        // printf("keyStreamLen : %d\n", keyStreamLen);

        //sum of distribution
        sumOfq2 = 0;
        for(int j = 0; j < 256; j++) {
            if (frequency[j] != 0) {
                sumOfq2 += pow(((double)frequency[j]/keyStreamLen), 2);
            }
        }
        //값 비교를 위한 저장
        Sums[i-1] = sumOfq2;
        // printf("sumOfq2 : %f \n", sumOfq2);
    }
    int Max = 0;
    for (int j = 0; j < 10; j++) {
        if (Sums[j] > Sums[Max]) { Max = j; }
        // printf("%f ", Sums[j]);
    }
    // printf("\nmax+1 : %d\n", Max+1);
    //max 키길이 결정
    //키 길이 결정

    int keyLen;
    unsigned char key[MAX_KEY_LEN];
    unsigned char keyStream[MAX_TEXT_LEN];

	for(int i = Max+1; i > 0 ; i--) {
        if ((Max+1) % i == 0) {
            //max키길이의 약수가 key 길이라고 가정
            keyLen = i;
            // printf("\nkeyLen이 %d 일 때\n", keyLen);
            keyStreamLen = 0;
            // 키의 내용을 찾기

            for (int k = 0; k < keyLen; k++) {
                // printf("\n------%d의 배수번째 글자 확인------\n", k+1);
                double sums[256];
                for (int j = 0; j < 256; j++) { sums[j] = 0; }
                for (unsigned int j = 0x00; j <= 0xff; j++) {
                    // printf("\n---%#x일 때---\n", j);
                    keyStreamLen = 0;
                    for (int l = k; l < len; l+=keyLen) {
                        keyStream[keyStreamLen] = ciphertxt[l] ^ j;
                        // 각 스트림에 대해서 256개의 키를 가지고 XOR을 수행
                        // printf("%c", keyStream[a]);
                        keyStreamLen++;
                    }
                    for (int l = 0; l < 256; l++) { frequency[l] = 0; }
                    for (int l = 0; l < keyStreamLen; l++) { frequency[keyStream[l]]++; }
                    // for (int l = 0; l < 256; l++) { printf("%d ", frequency[l]); }
                    // printf("\n");

                    // 각각에 대해 index of coincidence를 계산
                    sumOfq2 = 0;
                    for(int l = 97; l < 123; l++) {
                        if (frequency[l] != 0) {
                            sumOfq2 += pow(((double)frequency[l]/keyStreamLen), 2);
                        }
                    }
                    // printf("\nsumOfq2 : %f\n", sumOfq2);
                    sums[j] = sumOfq2;
                    // 각 알파벳에 대해 개수와 비율의 곱을 더한 값과 0.065 사이의 차액이 작은 것을 키로 선택
                }
                int max = 0;
                for (int j = 0; j < 256; j++) {
                    if ( sums[j] > sums[max] ) { max = j; }
                    // if ( sums[j] != 0) { printf("%d : %f ", j, sums[j]); }
                }
                key[k] = max;
            }

            // printf("\n\n");

            //복호화
            dycrypt(ciphertxt, len, key, keyLen, plaintxt);
            int confirm;
            printf("\n\nWill you select this plaintxt? (yes-1 / no-2) : ");
            scanf("%d", &confirm);
            if (confirm == 1) {
                printf("\nPlease check the 'hw1_output.txt' file!");
                //파일에 쓰기
                for (int i = 0; i < keyLen; i++) { fprintf(fpOut, "0x%x ", key[i]); }
                fprintf(fpOut, "\n");
                for (int i = 0; i < len; i++) { fwrite(&plaintxt[i], sizeof(plaintxt[i]), 1, fpOut); }
                break;
            }
        }
        if (i == 1) {
            printf("\nSorry, cannot find key and plaintext ... ");
        }
	}

    //파일 입출력 스트림 닫기
    fclose ( fpIn );
    fclose ( fpOut ) ;
    return 0 ;
}