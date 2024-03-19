#include <stdio.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/err.h>


#define ENCRYPT 1
#define MAX_BUFFER 1024
#define DECRYPT 0


void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

int main(int argc, char **argv){

//int EVP_EncryptInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, const unsigned char *key, const unsigned char *iv);
//int EVP_EncryptFinal(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl);

//  int EVP_DecryptInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, const unsigned char *key, const unsigned char *iv);
//  int EVP_DecryptFinal(EVP_CIPHER_CTX *ctx, unsigned char *outm, int *outl);

//  int EVP_CipherInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, const unsigned char *key, const unsigned char *iv, int enc);
//  int EVP_CipherFinal(EVP_CIPHER_CTX *ctx, unsigned char *outm, int *outl);

     if(argc != 3){
        fprintf(stderr,"Invalid parameters. Usage: %s file_in file_out\n",argv[0]);
        exit(1);
    }

    FILE *f_in;
    if((f_in = fopen(argv[1],"r")) == NULL) {
            fprintf(stderr,"Couldn't open the input file, try again\n");
            abort();
    }

    FILE *f_out;
    if((f_out = fopen(argv[4],"wb")) == NULL) {
            fprintf(stderr,"Couldn't open the output file, try again\n");
            abort();
    }

    unsigned char key[] = "0123456789ABCDEF";
    unsigned char iv[]  = "0123456789ABCDEF";


    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CipherInit(ctx,EVP_aria_128_cbc(), key, iv, DECRYPT);
    
    int length;
    unsigned char plaintext[MAX_BUFFER+16];

    int n_read;
    unsigned char buffer[MAX_BUFFER];

    int update_len, final_len;
    int decrypted_len=0;

    while((n_read = fread(buffer,1,MAX_BUFFER,f_in)) > 0){
        if(!EVP_CipherUpdate(ctx,plaintext,&length,buffer,n_read))
            handle_errors();

        decrypted_len+=length;

        EVP_CipherFinal_ex(ctx,plaintext+decrypted_len,&final_len);
        decrypted_len+=final_len;

        if(fwrite(plaintext, 1, length,f_out) < length){
            fprintf(stderr,"Error writing the output file\n");
            abort();
        }
    }   

    EVP_CIPHER_CTX_free(ctx);

    printf("Plaintext lenght = %d\n",decrypted_len);
    for(int i = 0; i < decrypted_len; i++)
        printf("%2x", plaintext[i]);
    printf("\n");
    for(int i = 0; i < decrypted_len; i++)
        printf("%c", plaintext[i]);
    printf("\n");

    return 0;
}