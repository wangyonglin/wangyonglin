#include <SHA256WithRSA.h>
const char privateKey[] = "-----BEGIN RSA PRIVATE KEY-----\n"
                          "MIICXQIBAAKBgQDlLm5+Kosybacfp8hzjn1fl2wT7Au2lm5SEtz6r+/wwSfq5KfY\n"
                          "H8q1AO/C92IwEpplNbrqYmOXQu6P07mg0lQOCvE5sdtmAvD2ex3wCef8lWmgdh5q\n"
                          "Uo4OMcmoSz3IAp/7/FnMag1IelSfdronPBDxazp6NUmQZITsYK6CsEl/ewIDAQAB\n"
                          "AoGBAJkMdvF+i9Kzc6YqMC0rfQJ3Zs+vFOtsbmQVAMnQ8JWBCJ1O8d/c60wRQgyb\n"
                          "lFCyO7VXOmoIJqX/Jr2aER8bFtG+Yxy6jsMu3ynwMwbhcVmCWCmZoWuE5pZdEJk6\n"
                          "lOdOay7TkE45X/Wc7K9iZs2uuB7sylIvK/HVxxit6FGePa4RAkEA9e+VoAbxBv78\n"
                          "HyxRcStW+Kc3lmE4zYBGAb2IYx48UEN34nP5rI8Tusqsy7CZ3rvSMi1CpVlj2eQK\n"
                          "FU8FzVFyjwJBAO6PU9q7il8NtecdvYBkDErlCawSeCdk9s79helT0Mrg9cWaVWFO\n"
                          "n0UxgT55MPXWGdMRXUUOCNnMilaw/p7dKlUCQDpjGeu3GivmB2dDN0ad2nUIBftu\n"
                          "s3SeWoB5RdL6T6liiyi5DfJ4uV9kVKe7Epy9jIabFjJ5SWpmaDps21zGVGMCQQCB\n"
                          "HvK0IW3zpOgf/+jh5UUCBJYHnLeMGwm7X11rvQH1zW05Vx9/W565ROI/fjkR1qCD\n"
                          "rZJeHgqMWDlIUuR9+BdBAkAI8+JWgWLdWceXX9Puu4KNmGukx4GZw2n53vMKp0Fu\n"
                          "puQxMonRWTN+kA76cq8QIj8xuEBkdxy1NFRMEkGu675m\n"
                          "-----END RSA PRIVATE KEY-----";

const char publicKey[] = "-----BEGIN PUBLIC KEY-----\n"
                         "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDlLm5+Kosybacfp8hzjn1fl2wT\n"
                         "7Au2lm5SEtz6r+/wwSfq5KfYH8q1AO/C92IwEpplNbrqYmOXQu6P07mg0lQOCvE5\n"
                         "sdtmAvD2ex3wCef8lWmgdh5qUo4OMcmoSz3IAp/7/FnMag1IelSfdronPBDxazp6\n"
                         "NUmQZITsYK6CsEl/ewIDAQAB\n"
                         "-----END PUBLIC KEY-----";

int main()
{
    char *plaintext = "Hello, sha256 with rsa!";
    char *plaintexts = "Hello, sha256 with rsa!";
    char *signature = signMessage(strdup(privateKey), plaintext, strlen(plaintext));
    fprintf(stdout, "%s\n", signature);
    bool authentic = verifySignature(strdup(publicKey), plaintexts, strlen(plaintexts), signature);
    if (authentic)
    {
        fprintf(stdout, "\tAuthentic\r\n");
    }
    else
    {
        fprintf(stderr, "\tNot Authentic\r\n");
    }
}