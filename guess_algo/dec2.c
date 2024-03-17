#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <string.h>

#define BUF_SIZE 1024

// Function to decode Base64
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
    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Set key and iv
    unsigned char key[] = "0123456789ABCDEF";
    unsigned char iv[] = "0123456789ABCDEF";

    // Base64 encoded string
    char* base64_encrypted = "ZZJ+BKJNdpXA2jaX8Zg5ItRola18hi95MG8fA/9RPvg=";

    // Decode Base64
    int base64_length = strlen(base64_encrypted);
    char* encrypted = base64_decode((unsigned char*)base64_encrypted, base64_length);

    // Create cipher context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    // Set cipher type and mode
    const EVP_CIPHER* cipher_type = EVP_chacha20_poly1305();

    // Initialize decryption operation
    if (!EVP_CipherInit(ctx, cipher_type, key, iv, 0)) {
        printf("Initialization of the cipher context failed.\n");
        return 1;
    }

    // Perform decryption
    int decrypted_len;
    unsigned char decrypted[BUF_SIZE];
    if (!EVP_CipherUpdate(ctx, decrypted, &decrypted_len, (unsigned char*)encrypted, base64_length)) {
        printf("Decryption failed.\n");
        return 1;
    }

    // Finalize decryption
    int final_len;
    if (!EVP_CipherFinal(ctx, decrypted + decrypted_len, &final_len)) {
        printf("Finalization of the decryption failed.\n");
        return 1;
    }

    decrypted_len += final_len;

    // Add null terminator
    decrypted[decrypted_len] = '\0';

    // Print decrypted content
    printf("Decrypted content: %s\n", decrypted);

    // Cleanup
    EVP_CIPHER_CTX_free(ctx);
    free(encrypted);

    return 0;
}
