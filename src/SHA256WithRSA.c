#include <SHA256WithRSA.h>

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

void SHA256withRSA_Base64Encode(const unsigned char *buffer,
                  size_t length,
                  char **base64Text)
{
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    b64 = BIO_new(BIO_f_base64());
    // 控制 base64 输出字符串不换行
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_write(bio, buffer, length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    BIO_free_all(bio);
    *base64Text = (*bufferPtr).data;
}

size_t calcDecodeLength(const char *b64input)
{
    size_t len = strlen(b64input), padding = 0;

    if (b64input[len - 1] == '=' && b64input[len - 2] == '=') // last two chars are =
        padding = 2;
    else if (b64input[len - 1] == '=') // last char is =
        padding = 1;
    return (len * 3) / 4 - padding;
}

void SHA256withRSA_Base64Decode(const char *b64message, unsigned char **buffer, size_t *length)
{
    BIO *bio, *b64;
    int decodeLen = calcDecodeLength(b64message);
    *buffer = (unsigned char *)malloc(decodeLen + 1);
    (*buffer)[decodeLen] = '\0';
    bio = BIO_new_mem_buf(b64message, -1);
    b64 = BIO_new(BIO_f_base64());
    // 控制 base64 输出字符串不换行
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_push(b64, bio);
    *length = BIO_read(bio, *buffer, strlen(b64message));
    BIO_free_all(bio);
}

char *signMessage(char *privateKey, char *plaintext, int plainlenth)
{
    SHA256WithRSA *privateRSA = SHA256WithRSA_Private(privateKey);
    unsigned char *encMessage;
    char *base64Text;
    size_t encMessageLength;
    SHA256withRSA_Signature(privateRSA, (unsigned char *)plaintext, plainlenth, &encMessage, &encMessageLength);
    SHA256withRSA_Base64Encode(encMessage, encMessageLength, &base64Text);
    free(encMessage);
    return base64Text;
}

bool verifySignature(char *publicKey, char *plaintext, int plainlenth, char *signatureBase64)
{
    SHA256WithRSA *publicRSA = SHA256WithRSA_Public(publicKey);
    unsigned char *encMessage;
    size_t encMessageLength;
    bool authentic;
    SHA256withRSA_Base64Decode(signatureBase64, &encMessage, &encMessageLength);
    bool result = SHA256withRSA_Verify(publicRSA, encMessage, encMessageLength, plaintext, plainlenth, &authentic);
    return result & authentic;
}

