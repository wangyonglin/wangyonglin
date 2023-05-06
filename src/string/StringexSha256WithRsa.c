#include <StringexSha256WithRsa.h>
#include <StringexBase64.h>
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

Boolean Stringex_Sha256WithRSA_Signature(Stringex *base64String, Stringex apiclient_key, char *plainText)
{
    Boolean err = Boolean_false;
    RSA *privateRSA = NULL;
    BIO *privateBIO = NULL;
    if (plainText && apiclient_key.valuestring)
    {
        size_t plainTextSize = strlen(plainText);
        if ((privateBIO = BIO_new_file(apiclient_key.valuestring, "rb")))
        {

            if ((privateRSA = PEM_read_bio_RSAPrivateKey(privateBIO, &privateRSA, NULL, NULL)))
            {

                unsigned char *encMessage;
                size_t encMessageLength;

                if (RSASign(privateRSA, (unsigned char *)plainText, plainTextSize, &encMessage, &encMessageLength))
                {

                    if (StringexBase64Encrypt(base64String, encMessage, encMessageLength, false))
                    {
                        err = Boolean_true;
                    }
                }

                if (encMessage)
                    free(encMessage);
                RSA_free(privateRSA);
            }

            BIO_free(privateBIO);
        }
    }
    return err;
}


Boolean Stringex_Sha256WithRSA_Certificates_Verify(
    Stringex certificates,
    Stringex plainText,
    Stringex ciphertext)
{
    Boolean err = Boolean_false;
    if (!Stringex_IsString(certificates) && !Stringex_IsEmpty(certificates))
    {
        fprintf(stderr, "Stringex certificates 参数为空%d", __LINE__);
        return err;
    }
    if (!Stringex_IsString(plainText) && !Stringex_IsEmpty(plainText))
    {
        fprintf(stderr, "Stringex plainText 参数为空%d", __LINE__);
        return err;
    }
    if (!Stringex_IsString(ciphertext) && !Stringex_IsEmpty(ciphertext))
    {
        fprintf(stderr, "Stringex ciphertext 参数为空%d", __LINE__);
        return err;
    }
    RSA *privateRsa = RSA_new();
    BIO *bio = NULL;
    EVP_PKEY *pkey = NULL;
    X509 *x509 = NULL;
    if ((bio = BIO_new_mem_buf((void *)certificates.valuestring, certificates.valuelength)) == NULL)
    {
        fprintf(stderr, "\tBIO_new_mem_buf privateKey error\r\n");
        return err;
    }
    if ((x509 = PEM_read_bio_X509(bio, NULL, NULL, NULL)) == NULL)
    {
        fprintf(stderr, "\tPEM_read_bio_X509 error\r\n");
        return err;
    }
    if ((pkey = X509_get_pubkey(x509)) == NULL)
    {
        fprintf(stderr, "\tX509_get_pubkey error\r\n");
        return err;
    }
    if ((privateRsa = EVP_PKEY_get1_RSA(pkey)) == NULL)
    {
        fprintf(stderr, "\tEVP_PKEY_get1_RSA error\r\n");
        return err;
    }
    BIO_free_all(bio);
    bool authentic;
    Stringex encMessage = stringex_null_command;
    if (StringexBase64Decrypt(&encMessage, ciphertext.valuestring, ciphertext.valuelength, false))
    {
        bool result = RSAVerifySignature(privateRsa, encMessage.valuestring, encMessage.valuelength, plainText.valuestring, plainText.valuelength, &authentic);
        StringexDelete(encMessage);
        err = result & authentic;
    }
    if (privateRsa != NULL)
        RSA_free(privateRsa);
    if (pkey != NULL)
        EVP_PKEY_free(pkey);
    if (x509 != NULL)
        X509_free(x509);
    return err;
}
Boolean Stringex_Sha256WithRSA_Verify(Stringex apiclient_key, Stringex plainText, Stringex ciphertext)
{
    Boolean err = Boolean_false;
    RSA *publicRSA = RSA_new();
    BIO *publicBIO = NULL;
    if ((publicBIO = BIO_new_file(apiclient_key.valuestring, "rb")))
    {
        printf("\tBIO_new_file ok\r\n");
        if ((publicRSA = PEM_read_bio_RSAPrivateKey(publicBIO, &publicRSA, NULL, NULL)))
        {
            bool authentic;
            Stringex encMessage = stringex_null_command;
            printf("\tPEM_read_bio_RSAPublicKey ok\r\n");
            if (StringexBase64Decrypt(&encMessage, ciphertext.valuestring, ciphertext.valuelength, false))
            {
                hex_print(encMessage.valuestring, encMessage.valuelength);
                bool result = RSAVerifySignature(publicRSA, encMessage.valuestring, encMessage.valuelength, plainText.valuestring, plainText.valuelength, &authentic);

                StringexDelete(encMessage);
                err = result & authentic;
            }
            RSA_free(publicRSA);
        }
        else
        {
            printf("\topen_public_key failed to PEM_read_bio_RSAPublicKey!\r\n");
        }

        BIO_free(publicBIO);
    }

    return err;
}