#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef SHA256WHTIRSA_H
#define SHA256WithRSA RSA
SHA256WithRSA *SHA256WithRSA_RSAPrivateKey_file(const char *file);
SHA256WithRSA *SHA256WithRSA_Private(char *key);
SHA256WithRSA *SHA256WithRSA_Public(char *key);
bool SHA256withRSA_Signature(SHA256WithRSA *rsa,
                             const unsigned char *plaintext,
                             size_t plainlenth,
                             unsigned char **ciphertext,
                             size_t *cipherlenth);
bool SHA256withRSA_Verify(SHA256WithRSA *rsa,
                          unsigned char *ciphertext,
                          size_t cipherlenth,
                          const char *plaintext,
                          size_t plainlenth,
                          bool *Authentic);
char *signMessage(char *privateKey, char *plaintext, int plainlenth);
bool verifySignature(char *publicKey, char *plaintext, int plainlenth, char *signatureBase64);      
char* SHA256WithRSA_Signature_RSAPrivateKey_file(const char *file, char *plaintext, int plainlenth);              
#endif // !SHA256WHTIRSA_H
