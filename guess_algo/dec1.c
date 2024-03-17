#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <string.h>

#define ENCRYPT 1
#define DECRYPT 0


char* base64_decode(const unsigned char* input, int length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    char* buffer = (char*)malloc(length);
    memset(buffer, 0, length);

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf((void*)input, length);
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // Do not use newlines to flush buffer
    BIO_read(bio, buffer, length);

    BIO_free_all(bio);

    return buffer;
}

int main() {

    unsigned char key[] = "0123456789ABCDEF";
    unsigned char iv[] = "0123456789ABCDEF";
    unsigned char ciphertext_base64[] = "ZZJ+BKJNdpXA2jaX8Zg5ItRola18hi95MG8fA/9RPvg=";
    char* ciphertext_hex = base64_decode((unsigned char*)ciphertext_base64, strlen(ciphertext_base64));

    printf("%s\n\n", ciphertext_hex);

    char algos[][20] = {
        "EVP_aes_128_cbc",
        "EVP_aes_192_cbc",
        "EVP_aes_256_cbc",
        "EVP_des_cbc",
        "EVP_des_ede3_cbc",
        "EVP_bf_cbc",
        "EVP_rc4",
        "EVP_rc2_cbc",
        "EVP_camellia_128_cbc",
        "EVP_chacha20_poly1305"
    };

    EVP_CIPHER_CTX *ctx[10];

    for(int i=0; i<10; i++)
        ctx[i] = EVP_CIPHER_CTX_new();
   
    for (int iter = 0; iter < 10; iter++) {
        printf("-----ITER %d ------\nalgo: %s\n", iter, algos[iter]);

        switch (iter) {
            case 0: EVP_CipherInit(ctx[iter], EVP_aes_128_cbc(), key, iv, DECRYPT); break;
            case 1: EVP_CipherInit(ctx[iter], EVP_aes_192_cbc(), key, iv, DECRYPT); break;
            case 2: EVP_CipherInit(ctx[iter], EVP_aes_256_cbc(), key, iv, DECRYPT); break;
            case 3: EVP_CipherInit(ctx[iter], EVP_des_cbc(), key, iv, DECRYPT); break;
            case 4: EVP_CipherInit(ctx[iter], EVP_des_ede3_cbc(), key, iv, DECRYPT); break;
            case 5: EVP_CipherInit(ctx[iter], EVP_bf_cbc(), key, iv, DECRYPT); break;
            case 6: EVP_CipherInit(ctx[iter], EVP_rc4(), key, iv, DECRYPT); break;
            case 7: EVP_CipherInit(ctx[iter], EVP_rc2_cbc(), key, iv, DECRYPT); break;
            case 8: EVP_CipherInit(ctx[iter], EVP_camellia_128_cbc(), key, iv, DECRYPT); break;
            case 9: EVP_CipherInit(ctx[iter], EVP_chacha20_poly1305(), key, iv, DECRYPT); break;
        }

        // convert hexstring into bytes
        int ciphertext_len = strlen(ciphertext_hex) / 2;
        unsigned char ciphertext_binary[ciphertext_len];
        for (int i = 0; i < ciphertext_len; i++) {
            sscanf(&ciphertext_hex[2 * i], "%2hhx", &ciphertext_binary[i]);
        }

        unsigned char decrypted[ciphertext_len]; // may be larger than needed due to padding

        int update_len, final_len;
        int decrypted_len = 0;
        EVP_CipherUpdate(ctx[iter], decrypted, &update_len, ciphertext_binary, ciphertext_len);
        decrypted_len += update_len;

        EVP_CipherFinal_ex(ctx[iter], decrypted + decrypted_len, &final_len);
        decrypted_len += final_len;

        EVP_CIPHER_CTX_free(ctx[iter]);

        printf("Plaintext length = %d\n", decrypted_len);
        for (int i = 0; i < decrypted_len; i++)
            printf("%02x", decrypted[i]);
        printf("\n");
        for (int i = 0; i < decrypted_len; i++)
            printf("%c", decrypted[i]);
        printf("\n");

        if(!EVP_CipherInit(ctx,algorithm_name(), key, iv, ENCRYPT))
            printf("CRYPTO24{%s%s}\n", decrypted, algos[iter]);
        printf("-------------------\n\n");
    }

    return 0;
}
