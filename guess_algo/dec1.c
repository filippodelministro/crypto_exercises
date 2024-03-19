#include <stdio.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/err.h>


#define ENCRYPT 1
#define DECRYPT 0
#define MAX_BUFFER 1024


void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

int main(int argc, char **argv){

    unsigned char key[] = "0123456789ABCDEF";
    unsigned char iv[]  = "0123456789ABCDEF";

    if(argc != 3){
        fprintf(stderr,"Invalid parameters. Usage: %s file_in file_out\n",argv[0]);
        exit(1);
    }

    
    const int dim = 7;

    const EVP_CIPHER* algos[dim];
    
    algos[0] = EVP_aes_128_cbc();
    algos[1] = EVP_aes_192_cbc();
    algos[2] = EVP_aes_256_cbc();
    algos[3] = EVP_des_ede3_cbc();
    algos[4] = EVP_aria_128_cbc();
    algos[5] = EVP_aria_192_cbc();
    algos[6] = EVP_aria_256_cbc();

    for(int i=0; i< dim; i++){
        FILE *f_in;
        if((f_in = fopen(argv[1],"r")) == NULL) {
            fprintf(stderr,"Couldn't open the input file, try again\n");
            abort();
        }

        FILE *f_out;
        if((f_out = fopen(argv[2],"wb")) == NULL) {
            fprintf(stderr,"Couldn't open the output file, try again\n");
            abort();
        }


        printf("----------\niter: %d\n", i);

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        EVP_CipherInit(ctx,algos[i], key, iv, DECRYPT);
        
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

       

    }

    return 0;

}