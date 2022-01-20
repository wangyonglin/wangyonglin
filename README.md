# plaintext
1614902012
299A2E0ABC5E94FFBF53E35B9D88F5CC04EA0D48
plaintext 是明文的意思
ciphertext 是表示密文                             
设置APIV3密钥 OAmmYAIb7lHu23YeMPJqKmtOUbT3SDqW
AC_INIT(plaintext, 1.0,admin@wangyonglin.com)
AM_INIT_AUTOMAKE
autoscan
autoheader
automake --add-missing

SHA256withRSA_Signature
SHA256withRSA_Verify
SHA256withRSA_Base64Encode
SHA256withRSA_Base64Decode

int main()
{
    char *plaintext = "Hello, sha256 with rsa!";
    char *plaintexts = "Hello, sha256 with rsa!d";
    char *signature = signMessage(privateKey, plaintext, strlen(plaintext));
    bool authentic = verifySignature(publicKey, plaintexts, strlen(plaintexts), signature);
    if (authentic)
    {
        fprintf(stdout, "\tAuthentic\r\n");
    }
    else
    {
        fprintf(stderr, "\tNot Authentic\r\n");
    }
}