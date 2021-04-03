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
static const char MimeBase64[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};
 
int base64_encode(char *text, int numBytes, char **encodedText) {
    unsigned char input[3] = { 0,0,0 };
    unsigned char output[4] = { 0,0,0,0 };
    int   index, i, j, size;
    char *p, *plen;
    plen = text + numBytes - 1;
    size = (4 * (numBytes / 3)) + (numBytes % 3 ? 4 : 0) + 1;
    (*encodedText) = (char*)malloc(size);
    j = 0;
    for (i = 0, p = text; p <= plen; i++, p++) {
        index = i % 3;
        input[index] = *p;
        if (index == 2 || p == plen) {
            output[0] = ((input[0] & 0xFC) >> 2);
            output[1] = ((input[0] & 0x3) << 4) | ((input[1] & 0xF0) >> 4);
            output[2] = ((input[1] & 0xF) << 2) | ((input[2] & 0xC0) >> 6);
            output[3] = (input[2] & 0x3F);
            (*encodedText)[j++] = MimeBase64[output[0]];
            (*encodedText)[j++] = MimeBase64[output[1]];
            (*encodedText)[j++] = index == 0 ? '=' : MimeBase64[output[2]];
            (*encodedText)[j++] = index <  2 ? '=' : MimeBase64[output[3]];
            input[0] = input[1] = input[2] = 0;
        }
    }
    (*encodedText)[j] = '\0';
    return size;
}

void print_data(const char *tittle, const void* data, int len) {
	printf("%s : ", tittle);
	const unsigned char * p = (const unsigned char*)data;
	for (int i = 0; i < len; ++i) printf("%02x", *p++);
	printf("\n");
}

int main () {
    FILE * fp_input = fopen("/Users/choeeunseo/C-CPP/CS/Assignment2/PlaintextCiphertext.txt", "r");
    FILE * fp_output = fopen("keys.txt", "w");
    FILE * table = fopen("/Users/choeeunseo/C-CPP/CS/Assignment2/MD5.txt", "r");
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
    DES_cblock key = {0x00, }; // 하나씩 대입해보자(64비트)
    //키 파일 읽기
    char cdtKey1[3] = "";
    char word[20] = "";
    for (int i = 0; i < 8; i++) {
        fgets(cdtKey1, sizeof(cdtKey1), table);
        key[i] = strtol(cdtKey1, NULL, 16);
        printf("k : %02x\n", key[i]);
    }
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
            printf("%02x ", *e+j);
        }
        printf("\nidx : %d\n\n", idx);
    }

    /* AES key for Encryption and Decryption */
    unsigned char aes_key[16]={0x00, };
    //키 파일 읽기
    fseek(table, 0, SEEK_SET);
    char cdtKey2[3] = "";
    char word2[20] = "";
    for (int i = 0; i < 16; i++) {
        fgets(cdtKey2, sizeof(cdtKey2), table);
        aes_key[i] = strtol(cdtKey2, NULL, 16);
        printf("k : %02x\n", aes_key[i]);
    }

	/* Input data to encrypt */
	unsigned char aes_input[idx];
    for (int i = 0; i < idx; i++) {
        aes_input[i] = AESblock[i];
    }
	
	/* Init vector */
	unsigned char iv[AES_BLOCK_SIZE];
	memset(iv, 0x00, AES_BLOCK_SIZE);
	
	/* Buffers for Encryption and Decryption */
	unsigned char enc_out[sizeof(aes_input)];
	
	/* AES-128 bit CBC Encryption */
	AES_KEY enc_key;
	AES_set_encrypt_key(aes_key, sizeof(aes_key)*8, &enc_key);
	AES_cbc_encrypt(aes_input, enc_out, sizeof(aes_input), &enc_key, iv, AES_ENCRYPT);
	
	/* Printing and Verifying */
	print_data("Original ",aes_input, sizeof(aes_input)); // you can not print data as a string, because after Encryption its not ASCII
	print_data("Encrypted",enc_out, sizeof(enc_out));

    char* encodedText;
    base64_encode(enc_out, sizeof(enc_out), &encodedText);
    printf("\n%s\n", encodedText);
    printf("sizeof txt : %lu\n", strlen(encodedText));

    //쓰기
    // fprintf(fp_output, "%s\n", key1);
    // fprintf(fp_output, "%s", key2);
    // fprintf(fp_output, "%c", ch);
    if(fp_input) fclose(fp_input);
    if(fp_output) fclose(fp_output);
    
    return 0;
}