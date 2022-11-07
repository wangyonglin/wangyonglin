#if !defined(__STRING_SHA256WITHRSA_H__)
#define __STRING_SHA256WITHRSA_H__

#include <stdlib.h>
#include <stdbool.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

typedef RSA SHA256WithRSA_RSA;

typedef struct
{
    RSA *rsa;
    BIO * bio;
} SHA256WithRSA_Arguments_t;

int SHA256WithRSA_Base64Encode(const unsigned char *buffer,
                               size_t length,
                               char **base64Text);

size_t SHA256WithRSA_CalcDecodeLength(const char *b64input);

void SHA256WithRSA_Base64Decode(const char *b64message, unsigned char **buffer, size_t *length);

bool SHA256withRSA_Signature(SHA256WithRSA_Arguments_t *arguments,
                             const unsigned char *plaintext,
                             size_t plainlenth,
                             unsigned char **ciphertext,
                             size_t *cipherlenth);

bool SHA256withRSA_Verify(SHA256WithRSA_Arguments_t *arguments,
                          unsigned char *ciphertext,
                          size_t cipherlenth,
                          const char *plaintext,
                          size_t plainlenth,
                          bool *Authentic);

SHA256WithRSA_Arguments_t *SHA256WithRSA_Arguments_crt(void *buffer);

void SHA256WithRSA_Arguments_del(SHA256WithRSA_Arguments_t *arguments);

bool SHA256WithRSA_Arguments_set_PrivateKey(SHA256WithRSA_Arguments_t *arguments);

bool SHA256WithRSA_Arguments_set_PublicKey(SHA256WithRSA_Arguments_t *arguments);

#endif //!__SHA256WITHRSA__H__
