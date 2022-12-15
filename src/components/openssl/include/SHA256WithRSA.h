#if !defined(OPENSSL_SHA256WithRSA_H)
#define OPENSSL_SHA256WithRSA_H
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
/**
 * @brief Create a Private R S A object
 * 
 * @param keybuffer ///参数 buffer
 * @return RSA* 
 */
RSA *createPrivateRSA(const char *keybuffer);

bool RSASign(RSA *rsa,
             const unsigned char *Msg,
             size_t MsgLen,
             unsigned char **EncMsg,
             size_t *MsgLenEnc);



void Base64Encode(const unsigned char *buffer,
                  size_t length,
                  char **base64Text);
void Base64Decode(const char *b64message, unsigned char **buffer, size_t *length);
RSA *createPublicRSA(char *keybuffer);
bool RSAVerifySignature(RSA *rsa,
                        unsigned char *MsgHash,
                        size_t MsgHashLen,
                        const char *Msg,
                        size_t MsgLen,
                        bool *Authentic);
char *SHA256WithRSA_signature(const char *privateKey, char *plainText);
bool SHA256WithRSA_verify(char *publicKey, char *plainText, char *signatureBase64);
#endif