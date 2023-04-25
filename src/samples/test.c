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
 
const char *privateKey = "-----BEGIN CERTIFICATE-----\n\
MIID3DCCAsSgAwIBAgIUSW1+JmaIuU57d/nlAU1iPkztWQwwDQYJKoZIhvcNAQEL\n\
BQAwXjELMAkGA1UEBhMCQ04xEzARBgNVBAoTClRlbnBheS5jb20xHTAbBgNVBAsT\n\
FFRlbnBheS5jb20gQ0EgQ2VudGVyMRswGQYDVQQDExJUZW5wYXkuY29tIFJvb3Qg\n\
Q0EwHhcNMjExMDExMTAxMTQzWhcNMjYxMDEwMTAxMTQzWjBuMRgwFgYDVQQDDA9U\n\
ZW5wYXkuY29tIHNpZ24xEzARBgNVBAoMClRlbnBheS5jb20xHTAbBgNVBAsMFFRl\n\
bnBheS5jb20gQ0EgQ2VudGVyMQswCQYDVQQGDAJDTjERMA8GA1UEBwwIU2hlblpo\n\
ZW4wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDPFsXk1aPLiL+BGKim\n\
M2XK2A+xijqVuQT56dPi+erUDOyn2CzP2SPh9eptNAVOL7GLfzBlgIqlERrdwJcV\n\
ZfBB8MwiuWQ5exvPAjeOICrhos3A6Wp1yEMfnoVbOC/aWsaAUpOVBsAdxvJk8zUf\n\
hAI2zmlDx+/1jShtalOLUP8yhM2HjF4Ro7B3ZyrvUG6Kmse+3ayzW9G4QGIyhTf+\n\
NmpOUmhOjTygN4+147kqAIIF+ci0XjZ/EPw4MR48o12rniU3J8oFaoAHJwzKTMPI\n\
cATALMrLh5R4U/lA9sOFkc3mh6GkU0Fo9FUdUfBC5cNlxYoJAmSz/2TDnSGWFuV1\n\
JvHfAgMBAAGjgYEwfzAJBgNVHRMEAjAAMAsGA1UdDwQEAwIE8DBlBgNVHR8EXjBc\n\
MFqgWKBWhlRodHRwOi8vZXZjYS5pdHJ1cy5jb20uY24vcHVibGljL2l0cnVzY3Js\n\
P0NBPTFCRDQyMjBFNTBEQkMwNEIwNkFEMzk3NTQ5ODQ2QzAxQzNFOEVCRDIwDQYJ\n\
KoZIhvcNAQELBQADggEBABH437/nHroX7xJ0JqxMH4kL63kMSQy9LCYkHRNq+ahY\n\
qKIFj4R9cXvTo0kC5o2F55qmynyDOPUT+LmqVFD4akZmZunJAWec62RBIU+CTQy1\n\
1sC8PUbOG1uhTZK9toumfKbEArq9+LjQO2vj8gBhSrCOiIWbMjm6R301cN4Mz2UV\n\
XAYH7NFpxwt6IMxbNRz8ftoHwE36h9Ea83ynvmyguWLe+KlbKqfMhC9ukYsWfu4R\n\
I9D2p7ZHo6BW/Z54U/tHS3v7XF6gsZ9oAppS0/+X2D71b+ouNlG0N02lc5wkMxs9\n\
B894nJFGRXAGrGUKtHzL3HK0edFrrt6UiHna2vL7MGI=\n\
-----END CERTIFICATE-----";

 
#define PASS "40D48F7BBAB2157D75439356085DECD7" //口令
 
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
	if ((privateRsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL,NULL)) == NULL) 
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