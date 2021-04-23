#define MAX_PLAIN_LEN 500000
#define DES_BLOCK_SIZE 8
#define AES_BLOCK_SIZE 16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <stdint.h>

//DES encryption => AES encryption => Base64 encoding.
//this is from - [https://gist.github.com/barrysteyn/7308212](https://gist.github.com/barrysteyn/7308212)
int Base64Encode(const unsigned char* buffer, size_t length, char** b64text) {
    //Encodes a binary safe base 64 string
	BIO *bio, *b64;
	BUF_MEM *bufferPtr;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
	BIO_write(bio, buffer, length);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);
	BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free_all(bio);

	*b64text=(*bufferPtr).data;

	return (0); //success
}

int main () {
    FILE * fp_input = fopen("PlaintextCiphertext.txt", "r");
    FILE * fp_output = fopen("keys.txt", "w");
    FILE * table = fopen("MD5.txt", "r");

    char plaintxt[MAX_PLAIN_LEN];
    char ciphertxt[MAX_PLAIN_LEN];

    //파일 컨텐츠 읽기 및 복사
    int txtLen = 0;
    int DESblockNum = 0;
    int AESlen = 0;
    while (1) {
        fgets(plaintxt, 100, fp_input);
        txtLen = (int)strlen(plaintxt);
        fgets(ciphertxt, 100, fp_input);
        if (feof(fp_input) != 0) {
            break;
        }
    }
    DESblockNum = ceil((float)txtLen/8);
    AESlen = DESblockNum*DES_BLOCK_SIZE;

    //block 생성
    unsigned char DESblock[DESblockNum][DES_BLOCK_SIZE];
    unsigned char AESblock[100];
    for (int i = 0; i < DESblockNum; i++) {
        for (int j = 0; j < 8; j++) {
            if (plaintxt[i*8+j] != '\n') {
                DESblock[i][j] = plaintxt[i*8+j];
            } else {
                DESblock[i][j] = '\0';
            }
        }
    }

    //DES
    //암호화용 배열 생성
    unsigned char  in[DES_BLOCK_SIZE];
    unsigned char out[DES_BLOCK_SIZE];
    //키 생성
    DES_cblock key = {0x00, }; // 하나씩 대입해보자(64비트)
    //키 파일 읽기
    char cdtKey1[3] = "";
    char word[20] = "";
    for (int i = 0; i < 8; i++) {
        fgets(cdtKey1, sizeof(cdtKey1), table);
        key[i] = strtol(cdtKey1, NULL, 16);
    }
    for (int i = 0; i < 8; i++) {
        fgets(cdtKey1, sizeof(cdtKey1), table);
    }
    fgets(word, sizeof(word), table);
    DES_key_schedule keysched; // 8비트를 빼자(56비트)
    DES_set_key((DES_cblock *)key, &keysched);

    //Encrypt
    int idx=DESblockNum*DES_BLOCK_SIZE;
    for (int i = 0; i < DESblockNum; i++) {
        for (int j = 0; j < 8; j++) {
            in[j] = DESblock[i][j];
        }
        DES_ecb_encrypt((DES_cblock *)in,(DES_cblock *)out, &keysched, DES_ENCRYPT);
    }

    /* Input data to encrypt */
    int padIdx = 0;
    for (int i = 0; i < 100000; i++) { // 16 32 48 , 48-40 = 8;
        if (16*i < idx) {
            continue;
        }
        padIdx = 16*i;
        break;
    }
    unsigned char aes_input[padIdx];
    for (int i = 0; i < idx; i++) {
        aes_input[i] = AESblock[i];
    }
    for (int i = idx; i < padIdx; i++) {
        aes_input[i] = '\0';
    }

    //AES key
    unsigned char aes_key[16]={0x00, };
    fseek(table, 0, SEEK_SET);
    char cdtKey2[3] = "";
    char word2[20] = "";
    for (int i = 0; i < 16; i++) {
        fgets(cdtKey2, sizeof(cdtKey2), table);
        aes_key[i] = strtol(cdtKey2, NULL, 16);
    }
    fgets(word, sizeof(word), table);
    for (int i = 0; i < 16; i++) {
        fgets(cdtKey2, sizeof(cdtKey2), table);
        aes_key[i] = strtol(cdtKey2, NULL, 16);
    }
    fgets(word2, sizeof(word2), table);

    AES_KEY enc_key;
    AES_set_encrypt_key(aes_key, sizeof(aes_key)*8, &enc_key);

    //IV
    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    //Encrypt
    unsigned char enc_out[sizeof(aes_input)];
    AES_cbc_encrypt(aes_input, enc_out, sizeof(aes_input), &enc_key, iv, AES_ENCRYPT);

    char* base64EncodeOutput;
    const unsigned char* text=enc_out;
    Base64Encode(text, sizeof(text)*8, &base64EncodeOutput);

    //쓰기
    if (base64EncodeOutput != ciphertxt) {
        fprintf(fp_output, "%s\n", word);
        fprintf(fp_output, "%s", word2);
    }
    if(fp_input) fclose(fp_input);
    if(fp_output) fclose(fp_output);
    
    return 0;
}