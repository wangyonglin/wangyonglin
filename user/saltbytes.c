#include <saltbytes.h>



int saltbytes_init(saltbytes_t **saltbytes, char *plaintext)
{
    int ret;
    if ((*saltbytes) = (saltbytes_t *)malloc(sizeof(saltbytes_t)))
    {
        memset((*saltbytes), 0x00, sizeof(saltbytes_t));
        (*saltbytes)->plaintext = strdup(plaintext);

        mbedtls_entropy_init(&(*saltbytes)->entropy);
        mbedtls_ctr_drbg_init(&(*saltbytes)->ctr_drbg);
        if ((ret = mbedtls_ctr_drbg_seed(&(*saltbytes)->ctr_drbg, mbedtls_entropy_func, &(*saltbytes)->entropy,
                                         (*saltbytes)->plaintext,
                                         strlen((*saltbytes)->plaintext))) != 0)
        {
            printf(" failed\n  ! mbedtls_ctr_drbg_seed returned %d(-0x%04x)\n", ret, -ret);
            saltbytes_free(*saltbytes);
            return ret;
        }
        if ((ret = mbedtls_ctr_drbg_random(&(*saltbytes)->ctr_drbg, (*saltbytes)->saltbytes, sizeof((*saltbytes)->saltbytes))) != 0)
        {
            printf(" failed\n  ! mbedtls_ctr_drbg_random returned %d\n", ret);
            saltbytes_free(*saltbytes);
            return ret;
        }
        if ((ret = mbedtls_base64_encode((*saltbytes)->salttext, sizeof((*saltbytes)->salttext), &((*saltbytes)->saltback), (*saltbytes)->saltbytes, sizeof((*saltbytes)->saltbytes))) != 0)
        {
            printf(" failed\n  ! mbedtls_base64_encode returned %d\n", ret);
        }

        return ret;
    }
    return -1;
}

void saltbytes_free(saltbytes_t *saltbytes)
{
    if (saltbytes)
    {
        mbedtls_ctr_drbg_free(&(saltbytes)->ctr_drbg);
        mbedtls_entropy_free(&(saltbytes)->entropy);
        free(saltbytes);
    }
}

int saltbytes_sha256(saltbytes_t *saltbytes, unsigned char *ciphertext)
{
    int sha256_buffer_len = strlen(saltbytes->plaintext) + saltbytes->saltback;
    printf("\tsha256_buffer_len:[%d]\r\n", sha256_buffer_len);
    unsigned char sha256_buffer[sha256_buffer_len];
    bzero(sha256_buffer, sha256_buffer_len);
    strncat(sha256_buffer, (char *)saltbytes->salttext, saltbytes->saltback);
    strncat(sha256_buffer, (char *)saltbytes->plaintext, strlen(saltbytes->plaintext));
    printf("\tsha256_buffer:[%s]\r\n", sha256_buffer);
    mbedtls_sha256_context sha256_ctx;
    mbedtls_sha256_init(&sha256_ctx);
    mbedtls_sha256_starts(&sha256_ctx, 0); // 0表示传sha256 ， 1 表示传SHA-244
    mbedtls_sha256_update(&sha256_ctx, sha256_buffer, sha256_buffer_len);
    mbedtls_sha256_finish(&sha256_ctx, ciphertext);
    mbedtls_sha256_free(&sha256_ctx);
    return 0;
}