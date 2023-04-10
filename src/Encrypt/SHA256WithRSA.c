#include <Encrypt/SHA256WithRSA.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/pem.h>

size_t calcDecodeLength(const char *b64input);

RSA *createPrivateRSA(char *apiclient_key)
{

    if (apiclient_key)
    {
        BIO *keybio = BIO_new_file(apiclient_key, "rb");
        if (keybio)
        {
            RSA *rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
            if (rsa)
            {
                BIO_free(keybio);
                return rsa;
            }
            BIO_free(keybio);
        }
    }

    return NULL;
}

bool RSASign(RSA *rsa,
             const unsigned char *Msg,
             size_t MsgLen,
             unsigned char **EncMsg,
             size_t *MsgLenEnc)
{
    EVP_MD_CTX *m_RSASignCtx = EVP_MD_CTX_create();
    EVP_PKEY *priKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(priKey, rsa);
    if (EVP_DigestSignInit(m_RSASignCtx, NULL, EVP_sha256(), NULL, priKey) <= 0)
    {
        return false;
    }
    if (EVP_DigestSignUpdate(m_RSASignCtx, Msg, MsgLen) <= 0)
    {
        return false;
    }
    if (EVP_DigestSignFinal(m_RSASignCtx, NULL, MsgLenEnc) <= 0)
    {
        return false;
    }
    *EncMsg = (unsigned char *)malloc(*MsgLenEnc);
    if (EVP_DigestSignFinal(m_RSASignCtx, *EncMsg, MsgLenEnc) <= 0)
    {
        return false;
    }
    EVP_MD_CTX_destroy(m_RSASignCtx);
    return true;
}

size_t Base64Encode(const unsigned char *data,
                    size_t datasize,
                    char **base64Text)
{

    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, data, datasize);
    BIO_ctrl(bio, BIO_CTRL_FLUSH, 0, NULL);

    BUF_MEM *bptr = NULL;
    BIO_get_mem_ptr(bio, &bptr);
    if (*base64Text = (char *)malloc(bptr->length + 1))
    {
        memset(*base64Text, 0x00, bptr->length + 1);
        memcpy(*base64Text, bptr->data, bptr->length);
        BIO_free_all(bio);
        return bptr->length;
    }
    BIO_free_all(bio);

    return -1;
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
void Base64Decode(const char *b64message, unsigned char **buffer, size_t *length)
{
    BIO *bio, *b64;
    int decodeLen = calcDecodeLength(b64message);
    *buffer = (unsigned char *)malloc(decodeLen + 1);
    (*buffer)[decodeLen] = '\0';
    bio = BIO_new_mem_buf(b64message, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);
    *length = BIO_read(bio, *buffer, strlen(b64message));
    BIO_free_all(bio);
}

RSA *createPublicRSA(char *apiclient_key)
{
    if (apiclient_key)
    {
        BIO *keybio = BIO_new_file(apiclient_key, "rb");
        if (keybio)
        {
            RSA *rsa = PEM_read_bio_RSAPublicKey(keybio, NULL, NULL, NULL);
            if (rsa)
            {
                BIO_free(keybio);
                return rsa;
            }
            BIO_free(keybio);
        }
    }

    return NULL;
}
bool RSAVerifySignature(RSA *rsa,
                        unsigned char *MsgHash,
                        size_t MsgHashLen,
                        const char *Msg,
                        size_t MsgLen,
                        bool *Authentic)
{
    *Authentic = false;
    EVP_PKEY *pubKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pubKey, rsa);
    EVP_MD_CTX *m_RSAVerifyCtx = EVP_MD_CTX_create();
    if (EVP_DigestVerifyInit(m_RSAVerifyCtx, NULL, EVP_sha256(), NULL, pubKey) <= 0)
    {
        return false;
    }
    if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, Msg, MsgLen) <= 0)
    {
        return false;
    }
    int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx, MsgHash, MsgHashLen);
    if (AuthStatus == 1)
    {
        *Authentic = true;
        EVP_MD_CTX_destroy(m_RSAVerifyCtx);
        return true;
    }
    else if (AuthStatus == 0)
    {
        *Authentic = false;
        EVP_MD_CTX_destroy(m_RSAVerifyCtx);
        return true;
    }
    else
    {
        *Authentic = false;
        EVP_MD_CTX_destroy(m_RSAVerifyCtx);
        return false;
    }
}

bool sha256WithRSAVerify(char *apiclient_key, char *plainText, char *signatureBase64)
{
    RSA *publicRSA = createPublicRSA(apiclient_key);
    unsigned char *encMessage;
    size_t encMessageLength;
    bool authentic;
    Base64Decode(signatureBase64, &encMessage, &encMessageLength);
    bool result = RSAVerifySignature(publicRSA, encMessage, encMessageLength, plainText, strlen(plainText), &authentic);
    return result & authentic;
}

bool sha256WithRSASignature(char *apiclient_key, char *plainText, size_t plainTextSize, char **base64Text)
{
    RSA *privateRSA = createPrivateRSA(apiclient_key);

    if (privateRSA)
    {
        fprintf(stdout, "privateRSA YES\n");

        unsigned char *encMessage;
        size_t encMessageLength;
        RSASign(privateRSA, (unsigned char *)plainText, plainTextSize, &encMessage, &encMessageLength);
        Base64Encode(encMessage, encMessageLength, base64Text);
        if (encMessage)
            free(encMessage);
        RSA_free(privateRSA);
        return true;
    }
    else
    {
        fprintf(stdout, "privateRSA NULL\n");
    }

    return false;
}

bool Sha256WithRSASignatureEx(string_by_t apiclient_key, char *plainText, size_t plainTextSize, char **base64Text)
{

    RSA *privateRSA = NULL;
    BIO *privateBIO = NULL;
    if ((privateBIO = BIO_new_file(apiclient_key.valuestring, "rb")))
    {

        if ((privateRSA = PEM_read_bio_RSAPrivateKey(privateBIO, &privateRSA, NULL, NULL)))
        {

            unsigned char *encMessage;
            size_t encMessageLength;

            if (RSASign(privateRSA, (unsigned char *)plainText, plainTextSize, &encMessage, &encMessageLength))
            {
                Base64Encode(encMessage, encMessageLength, base64Text);
                if (encMessage)
                    free(encMessage);
                RSA_free(privateRSA);
                BIO_free(privateBIO);
                return true;
            }
            RSA_free(privateRSA);
        }

        BIO_free(privateBIO);
    }

    return false;
}