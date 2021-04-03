#define MAX_PLAIN_LEN 500000
#define DES_BLOCK_SIZE 8
#define AES_BLOCK_SIZE 16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
//DES encryption => AES encryption => Base64 encoding.

int main () {
    FILE * fp_input = fopen("/Users/choeeunseo/C-CPP/CS/Assignment2/PlaintextCiphertext.txt", "r");
    FILE * fp_output = fopen("keys.txt", "w");
    char plaintxt[100];
    char ciphertxt[100];

    //예외처리
    if (fp_input == NULL) {
        return -1;
    }

    //파일 컨텐츠 읽기 및 복사
    int txtLen = 0;
    int DESblockNum = 0;
    int AESlen = 0;
    while (1) {
        fgets(plaintxt, 100, fp_input);
        printf("%s\n", plaintxt);
        txtLen = (int)strlen(plaintxt);
        fgets(ciphertxt, 100, fp_input);
        printf("%s\n", ciphertxt);
        if (feof(fp_input) != 0) {
            break;
        }
    }
    printf("\ntxtLen : %d\n", txtLen);
    DESblockNum = ceil((float)txtLen/8);
    AESlen = DESblockNum*DES_BLOCK_SIZE;
    printf("DESblockNum : %d\n", DESblockNum);
    printf("AESlen : %d\n", AESlen);

    //block 생성
    unsigned char DESblock[10][10];
    unsigned char AESblock[100];
    for (int i = 0; i < DESblockNum; i++) {
        for (int j = 0; j < 8; j++) {
            if (plaintxt[i*8+j] != '\n') {
                DESblock[i][j] = plaintxt[i*8+j];
                printf("%c", DESblock[i][j]);
            } else {
                DESblock[i][j] = '\0';
            }
        }
        printf("\n");
    }

    //DES
    //암호화용 배열 생성
    unsigned char  in[DES_BLOCK_SIZE];
    unsigned char out[DES_BLOCK_SIZE];
    unsigned char* e = out;
    //키 생성
    DES_cblock key = "coders"; // 하나씩 대입해보자(64비트)
    DES_key_schedule keysched; // 8비트를 빼자(56비트)
    DES_set_key((DES_cblock *)key, &keysched);
    //Encrypt
    int idx=0;
    for (int i = 0; i < DESblockNum; i++) {
        for (int j = 0; j < 8; j++) {
            in[j] = DESblock[i][j];
        }
        printf("\nPlaintext(in): ");
        for (int j = 0; j < 8; j++) {
            printf("%c", DESblock[i][j]);
        }
        DES_ecb_encrypt((DES_cblock *)in,(DES_cblock *)out, &keysched, DES_ENCRYPT);
        printf("\n");
        printf("Ciphertext(out): ");
        for (int j = 0; j < 8; j++) {
            AESblock[idx++] = *e+j;
            printf("[%02x] ", *e+j);
        }
        printf("\n\nidx : %d\n", idx);
    }
/*
    //AES
    //암호화용 배열 생성
    const unsigned char* aes_input = AESblock;
    unsigned char aes_output[sizeof(aes_input)];
    unsigned char* hex = aes_output;
    // 키 생성
    const static unsigned char aes_key[]="piewtf";
    AES_KEY enc_key;
    AES_set_encrypt_key(aes_key, sizeof(aes_key)*8, &enc_key);
    
    // Init vector 세팅
    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);
    
    AES_cbc_encrypt(aes_input, aes_output, sizeof(aes_input), &enc_key, iv, AES_ENCRYPT);
*/

    //쓰기
    // fprintf(fp_output, "%s\n", key1);
    // fprintf(fp_output, "%s", key2);
    // fprintf(fp_output, "%c", ch);
    if(fp_input) fclose(fp_input);
    if(fp_output) fclose(fp_output);
    
    return 0;
}