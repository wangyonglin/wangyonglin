#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h> 
#include <openssl/crypto.h> 

/*
 * 
 */ 

#define PRIVATE_KEY_PATH ("/home/wangyonglin/github/wangyonglin/conf/wechat.cert")

#define SHA_WHICH        NID_sha256
#define WHICH_DIGEST_LENGTH    SHA256_DIGEST_LENGTH


void printHex(unsigned char *md, int len)
{

    int i = 0;
    for (i = 0; i < len; i++)
    {
        printf("%02x", md[i]);
    }

    printf("\n");
}

/*读取私钥*/
RSA* ReadPrivateKey(char* p_KeyPath)
{   
    FILE *fp = NULL; 
    RSA  *priRsa = NULL;

    printf("PrivateKeyPath[%s] \n", p_KeyPath);

    /*  打开密钥文件 */
    if(NULL == (fp = fopen(p_KeyPath, "r")))
    {
        printf( "fopen[%s] failed \n", p_KeyPath);
        return NULL;
    }
    /*  获取私钥 */
    priRsa = PEM_read_RSAPrivateKey(fp, NULL, NULL,NULL);
    if(NULL == priRsa)
    {
        ERR_print_errors_fp(stdout);
        printf( "PEM_read_RSAPrivateKey\n");
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    return priRsa;
}

int test_RSA_sign(void)
{
    char *data = "china";
    char buf[128] = {0};
    RSA *privKey = NULL;
    int nOutLen = sizeof(buf);
    int nRet = 0;

    //对数据进行sha256算法摘要
    unsigned char md[WHICH_DIGEST_LENGTH];

    SHA256((unsigned char *)data, strlen(data), md);
    printHex(md, WHICH_DIGEST_LENGTH);

    privKey = ReadPrivateKey(PRIVATE_KEY_PATH);
    if (!privKey) 
    {  
        ERR_print_errors_fp (stderr);    
        return -1;  
    }


    /* 签名 */
    nRet = RSA_sign(SHA_WHICH, md, WHICH_DIGEST_LENGTH, buf, &nOutLen, privKey);
    if(nRet != 1)
    {
        printf("RSA_sign err !!! \n");    
        goto quit;
    }
    printf("RSA_sign len = %d:", nOutLen);
    printHex(buf, nOutLen);


quit:
    RSA_free(privKey);

    return 0;
}


int main(int argc, char *argv[])
{
    test_RSA_sign();
    return 0;
}
