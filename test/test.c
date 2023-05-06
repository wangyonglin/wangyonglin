#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
 
const char *publicKey = "-----BEGIN PUBLIC KEY-----\n\
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDnal1HozHfmZ3B1TITmbjCNKOs\n\
49S+urgJ2P0/T36qN5w+r1jGhZKr54QDI5oXEk+9arlKxDW8kufwGjaTV3i3hyGS\n\
jYv4wNXhPeZAyAQ1vlloLMT6oA0PKe9/l8+mAr1QPEW9fMixAc/0UzPVospjkpfr\n\
YULcrKcH2Oaou5DZ0QIDAQAB\n\
-----END PUBLIC KEY-----";
 
const char *privateKey = "-----BEGIN RSA PRIVATE KEY-----\n\
Proc-Type: 4,ENCRYPTED\n\
DEK-Info: DES-EDE3-CBC,DF3BD9835CA1186D\n\
\n\
pomWi9/hjscwDDzH5CEpcj8nCumOQpl/2Gk2YynA47qfhxt12glNjgWl5Eaevk2L\n\
bG1t85sPqEvYxAe+ZxZdP6fot+sAg4SUUwSvBMwa7s3XjVhHjf/+hOIjb0skHvp/\n\
p0eOoUgytX7FrNNYEpUFI+eiPob79fgQMq/rypGJ//G6GXLMYixWw2+PyPa1x2PQ\n\
WdBaTpZK3gmDqmu6jR3ieKOahVVO4fEGB5etvB5i1aAh0mT4Wu+ejv2LgIRr2xor\n\
r8LkQZvI/TryZ0sNLe7LlC1bz/Hw8hLBDPprhWaUcSEk6MMgh3LKA2y/pGpFdIYN\n\
Ncj/c+YqEsO+I0KOtPQ1fXlXd1hH1H1rkJxuaNanF0UInUuupV3fP+7cvmfyHM4m\n\
aix8ROt1/Ghau41JDZGYmwk2qgKjUw4zz3eYOMQKl6row3pzhDxbvoMp0Qvfje1J\n\
RYpKMy8skG9pY1l4i1CC98aESC2a7FzjUNcY3f5Jt+QznO15xXxxuJZ8+xNqtIh2\n\
U348rlrQ8OxS1YBJCr+wjesdBdQAiY6X1YB9tljPs7AhlTLo78pHtQac521xOA8j\n\
IcbfkuTIrMIwYBOtM6SJHkB1TgPdPWx+haEy79Ct2yDnvpPqOiFz8i8TG8AQY53l\n\
5xKxxJ9CmPqw+Ua3DAWPaxAMaJFteRbl5Lv/2MvxV9Mu3T0W4B3ij+Gg5aw81v5Y\n\
KTH2KxruYAF5Q70QG8CAR8Vkvdczw940y8nb9pvcixmqYcaaeM9DLaTbycn/AeCt\n\
3UM0R0vvu039Ix5uhXUtVMjhTeUnvNObwEcKM8Grv1oPV3zmTJ5hJg==\n\
-----END RSA PRIVATE KEY-----";
 
#define PASS "8888" //口令
 
 
int main(int argc, char *argv[])
{
	BIO *bio = NULL;
	RSA *publicRsa = NULL;
	RSA *privateRsa = NULL;
	if ((bio = BIO_new_mem_buf((void *)publicKey, -1)) == NULL)
	{
		printf("BIO_new_mem_buf publicKey error\n");
		return -1;
	} 	
   
	if ((publicRsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL)) == NULL) 
	{
		printf("PEM_read_bio_RSA_PUBKEY error\n");
		return -1;
	}
	BIO_free_all(bio);
	
	if ((bio = BIO_new_mem_buf((void *)privateKey, -1)) == NULL)
	{
		printf("BIO_new_mem_buf privateKey error\n");
		return -1;
	}
	OpenSSL_add_all_algorithms();//密钥有经过口令加密需要这个函数
	if ((privateRsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, (char *)PASS)) == NULL) 
	{
		printf("PEM_read_RSAPrivateKey error\n");
		return -1;
	}
	BIO_free_all(bio);
	
	unsigned char *source = (unsigned char *)"123456789";
		
	int rsa_len = RSA_size(publicRsa);
 
	unsigned char *encryptMsg = (unsigned char *)malloc(rsa_len);
	memset(encryptMsg, 0, rsa_len);
 		
	int len = rsa_len - 11;
 		
	if (RSA_public_encrypt(len, source, encryptMsg, publicRsa, RSA_PKCS1_PADDING) < 0)
		printf("RSA_public_encrypt error\n");
	else 
	{
		rsa_len = RSA_size(privateRsa);
		unsigned char *decryptMsg = (unsigned char *)malloc(rsa_len);
		memset(decryptMsg, 0, rsa_len);
	    
		int mun =  RSA_private_decrypt(rsa_len, encryptMsg, decryptMsg, privateRsa, RSA_PKCS1_PADDING);
	 
		if ( mun < 0)
			printf("RSA_private_decrypt error\n");
		else
			printf("RSA_private_decrypt %s\n", decryptMsg);
	}	
	
	RSA_free(publicRsa);
	RSA_free(privateRsa);
	return 0;
}