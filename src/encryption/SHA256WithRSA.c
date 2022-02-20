#include <wangyonglin/core.h>
#include <wangyonglin/config.h>
#include <encryption/SHA256WithRSA.h>
#include <encryption/Base64.h>
SHA256WithRSA *SHA256WithRSA_RSAPrivateKey_file(const char *file)
{
    RSA *rsa = NULL;
    BIO *bio = BIO_new(BIO_s_file());
    if (bio == NULL)
    {
        return NULL;
    }
    BIO_read_filename(bio,file);
    rsa = PEM_read_bio_RSAPrivateKey(bio, &rsa, NULL, NULL);
    return rsa;
}
SHA256WithRSA *SHA256WithRSA_Private(char *key)
{
    RSA *rsa = NULL;
    BIO *bio = BIO_new_mem_buf((void *)key, -1);
    if (bio == NULL)
    {
        return 0;
    }
    rsa = PEM_read_bio_RSAPrivateKey(bio, &rsa, NULL, NULL);
    return rsa;
}

SHA256WithRSA *SHA256WithRSA_Public(char *key)
{
    RSA *rsa = NULL;
    BIO *bio;
    bio = BIO_new_mem_buf((void *)key, -1);
    if (bio == NULL)
    {
        return 0;
    }
    rsa = PEM_read_bio_RSA_PUBKEY(bio, &rsa, NULL, NULL);
    return rsa;
}

bool SHA256withRSA_Signature(SHA256WithRSA *rsa,
                             const unsigned char *plaintext,
                             size_t plainlenth,
                             unsigned char **ciphertext,
                             size_t *cipherlenth)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_create();
    EVP_PKEY *pky = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pky, rsa);
    if (EVP_DigestSignInit(ctx, NULL, EVP_sha256(), NULL, pky) <= 0)
    {
        return false;
    }
    if (EVP_DigestSignUpdate(ctx, plaintext, plainlenth) <= 0)
    {
        return false;
    }
    if (EVP_DigestSignFinal(ctx, NULL, cipherlenth) <= 0)
    {
        return false;
    }
    *ciphertext = (unsigned char *)malloc(*cipherlenth);
    if (EVP_DigestSignFinal(ctx, *ciphertext, cipherlenth) <= 0)
    {
        return false;
    }
    EVP_MD_CTX_free(ctx);
    return true;
}

bool SHA256withRSA_Verify(SHA256WithRSA *rsa,
                          unsigned char *ciphertext,
                          size_t cipherlenth,
                          const char *plaintext,
                          size_t plainlenth,
                          bool *Authentic)
{
    *Authentic = false;
    EVP_PKEY *pubKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pubKey, rsa);
    EVP_MD_CTX *ctx = EVP_MD_CTX_create();

    if (EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, pubKey) <= 0)
    {
        return false;
    }
    if (EVP_DigestVerifyUpdate(ctx, plaintext, plainlenth) <= 0)
    {
        return false;
    }
    int AuthStatus = EVP_DigestVerifyFinal(ctx, ciphertext, cipherlenth);
    if (AuthStatus == 1)
    {
        *Authentic = true;
        EVP_MD_CTX_free(ctx);
        return true;
    }
    else if (AuthStatus == 0)
    {
        *Authentic = false;
        EVP_MD_CTX_free(ctx);
        return true;
    }
    else
    {
        *Authentic = false;
        EVP_MD_CTX_free(ctx);
        return false;
    }
}


char* SHA256WithRSA_Signature_RSAPrivateKey_file(const char *file, char *plaintext, int plainlenth)
{
    SHA256WithRSA *privateRSA = SHA256WithRSA_RSAPrivateKey_file(file);
    unsigned char *encMessage;
    char *base64Text;
    size_t encMessageLength;
    SHA256withRSA_Signature(privateRSA, (unsigned char *)plaintext, plainlenth, &encMessage, &encMessageLength);
    Base64Encode(encMessage, encMessageLength, &base64Text);
    free(encMessage);
    return base64Text;
}

char *signMessage(char *privateKey, char *plaintext, int plainlenth)
{
    SHA256WithRSA *privateRSA = SHA256WithRSA_Private(privateKey);
    unsigned char *encMessage;
    char *base64Text;
    size_t encMessageLength;
    SHA256withRSA_Signature(privateRSA, (unsigned char *)plaintext, plainlenth, &encMessage, &encMessageLength);
    Base64Encode(encMessage, encMessageLength, &base64Text);
    free(encMessage);
    return base64Text;
}

bool verifySignature(char *publicKey, char *plaintext, int plainlenth, char *signatureBase64)
{
    SHA256WithRSA *publicRSA = SHA256WithRSA_Public(publicKey);
    unsigned char *encMessage;
    size_t encMessageLength;
    bool authentic;
    Base64Decode(signatureBase64, &encMessage, &encMessageLength);
    bool result = SHA256withRSA_Verify(publicRSA, encMessage, encMessageLength, plaintext, plainlenth, &authentic);
    return result & authentic;
}
