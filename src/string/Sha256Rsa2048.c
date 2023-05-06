#include <Sha256Rsa2048.h>
#include <StringexBase64.h>

Boolean Sha256Rsa2048_Signature(
    Stringex *ciphertext,
    Stringex certificates,
    char *plainText)
{
    Boolean err = Boolean_false;
    //  RSA *privateRSA = NULL;
    BIO *privateBIO = NULL;
    if (!Stringex_IsString(certificates) && !Stringex_IsEmpty(certificates))
    {
        fprintf(stderr, "Stringex certificates 参数为空%d", __LINE__);
        return err;
    }

    if (plainText)
    {
        size_t plainTextSize = strlen(plainText);
        RSA *publicRsa = NULL;
        BIO *bio = NULL;
        if ((bio = BIO_new_mem_buf((void *)certificates.valuestring, certificates.valuelength)) == NULL)
        {
            fprintf(stderr, "\tBIO_new_mem_buf privateKey error\r\n");
            return err;
        }
        if ((publicRsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL)) == NULL)
        {
            fprintf(stderr, "PEM_read_bio_RSA_PUBKEY error\n");
            return err;
        }
        BIO_free_all(bio);

        size_t encMessageLength = RSA_size(publicRsa);
        unsigned char *encMessage = (unsigned char *)malloc(encMessageLength);
        memset(encMessage, 0, encMessageLength);
        int len = encMessageLength - 11;

        if (RSA_public_encrypt(len, plainText, encMessage, publicRsa, RSA_PKCS1_PADDING) > 0)
        {
            if (Base64Encrypt(ciphertext, encMessage, encMessageLength, false))
            {
                err = Boolean_true;
            }
            else
            {
                fprintf(stderr, "StringexBase64Encrypt error\n");
            }
        }
        else
        {
            fprintf(stderr, "RSA_public_encrypt error\n");
        }

        if (encMessage)
            free(encMessage);
        RSA_free(publicRsa);
    }
    return err;
}

Boolean Sha256Rsa2048_Verify(
    Stringex ciphertext,
    Stringex plainText,
    Stringex certificates)
{
    Boolean err = Boolean_false;
    BIO *privateRsa = NULL;
    if (!Stringex_IsString(certificates) && !Stringex_IsEmpty(certificates))
    {
        fprintf(stderr, "Stringex certificates 参数为空%d", __LINE__);
        return err;
    }
    if (!Stringex_IsString(ciphertext) && !Stringex_IsEmpty(ciphertext))
    {
        fprintf(stderr, "Stringex ciphertext 参数为空%d", __LINE__);
        return err;
    }
    if (!Stringex_IsString(plainText) && !Stringex_IsEmpty(plainText))
    {
        fprintf(stderr, "Stringex plainText 参数为空%d", __LINE__);
        return err;
    }

    BIO *bio = NULL;
    if ((bio = BIO_new_mem_buf((void *)certificates.valuestring, certificates.valuelength)) == NULL)
    {
        fprintf(stderr, "\tBIO_new_mem_buf privateKey error\r\n");
        return err;
    }
    if ((privateRsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL)) == NULL)
    {
        fprintf(stderr, "PEM_read_bio_RSAPrivateKey error\n");
        return err;
    }
    BIO_free_all(bio);

    size_t privateKeyLength = RSA_size(privateRsa);
    Stringex encMessage;
    Base64Decrypt(&encMessage, ciphertext.valuestring, ciphertext.valuelength, false);
    if (RSA_private_decrypt(privateKeyLength, encMessage.valuestring, plainText.valuestring, privateRsa, RSA_PKCS1_PADDING) > 0)
    {
        fprintf(stdout, "RSA_private_decrypt %s\n", plainText.valuestring);
        err = Boolean_true;
    }
    else
    {
        fprintf(stderr, "RSA_private_decrypt error\n");
    }

    RSA_free(privateRsa);
    StringexDelete(encMessage);

    return err;
}