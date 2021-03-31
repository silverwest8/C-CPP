#define BLOCK_SIZE 16
#define BUFSIZE 64
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>

void print_data(const char *tittle, const void* data, int len) {
	printf("%s : ", tittle);
	const unsigned char * p = (const unsigned char*)data;
	int i = 0;
	for (; i<len; ++i) {
		printf("%02X ", *p++);
    }
	printf("\n");
}

int main () {
    FILE * fp_input = fopen("PlaintextCiphertext.txt", "r");
    FILE * fp_output = fopen("keys.txt", "w");
    char * txt;
    char block[BLOCK_SIZE+1];
    char key1[8] = "coders"; // 64bits
    char key2[16] = "piewtf"; //128bits
    while ( feof(fp_input) == 0) {
        txt = fgets(block, BLOCK_SIZE+1, fp_input);
        printf("%s\n", txt);
    }
    /* AES key for Encryption and Decryption */
    const static unsigned char aes_key[]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
    /* Input data to encrypt */
    unsigned char aes_input[]={0x0,0x1,0x2,0x3,0x4,0x5};
    
    /* Init vector */
    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);
    
    /* Buffers for Encryption and Decryption */
    unsigned char enc_out[sizeof(aes_input)];
    unsigned char dec_out[sizeof(aes_input)];
    
    /* AES-128 bit CBC Encryption */
    AES_KEY enc_key, dec_key;
    AES_set_encrypt_key(aes_key, sizeof(aes_key)*8, &enc_key);
    AES_cbc_encrypt(aes_input, enc_out, sizeof(aes_input), &enc_key, iv, AES_ENCRYPT);
    /* AES-128 bit CBC Decryption */
    memset(iv, 0x00, AES_BLOCK_SIZE); // don't forget to set iv vector again, else you can't decrypt data properly
    AES_set_decrypt_key(aes_key, sizeof(aes_key)*8, &dec_key); // Size of key is in bits
    AES_cbc_encrypt(enc_out, dec_out, sizeof(aes_input), &dec_key, iv, AES_DECRYPT);
    
    /* Printing and Verifying */
    // you can not print data as a string, because after Encryption its not ASCII
    print_data("\n Original ",aes_input, sizeof(aes_input));
    print_data("\n Encrypted",enc_out, sizeof(enc_out));    
    print_data("\n Decrypted",dec_out, sizeof(dec_out));

    unsigned char in[BUFSIZE], out[BUFSIZE], back[BUFSIZE];
    unsigned char *e = out;

    DES_cblock key;
    DES_cblock seed = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    DES_key_schedule keysched;
 
    memset(in, 0, sizeof(in));
    memset(out, 0, sizeof(out));
    memset(back, 0, sizeof(back));
 
    RAND_seed(seed, sizeof(DES_cblock));
 
    DES_random_key(&key);
 
    DES_set_key((DES_cblock *)key, &keysched);
 
    /* 8 bytes of plaintext */
    // strcpy(in, "HillTown");
 
    printf("Plaintext: [%s]\n", in);
 
    DES_ecb_encrypt((DES_cblock *)in,(DES_cblock *)out, &keysched, DES_ENCRYPT);
 
    printf("Ciphertext:");
    while (*e) printf(" [%02x]", *e++);
    printf("\n");
 
    DES_ecb_encrypt((DES_cblock *)out,(DES_cblock *)back, &keysched, DES_DECRYPT);
 
    printf("Decrypted Text: [%s]\n", back);
 
    //쓰기
    // fprintf(fp_output, "%s\n", key1);
    // fprintf(fp_output, "%s", key2);
    // fprintf(fp_output, "%c", ch);
    fclose(fp_input);
    fclose(fp_output);
    return 0;
}