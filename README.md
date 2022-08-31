/usr/local/wangyonglin/bin/wangyonglin -d -c /usr/local/wangyonglin/conf/wangyonglin.conf
#Makefile.am文件
bin_PROGRAMS = xxx
#bin_PROGRAMS 表示指定要生成的可执行应用程序文件，这表示可执行文件在安装时需要被安装到系统
#中；如果只是想编译，不想被安装到系统中，可以用noinst_PROGRAMS来代替

xxx_SOURCES = a.c b.c c.c main.c d.c xxx.c
#xxx_SOURCES表示生成可执行应用程序所用的源文件，这里注意，xxx_是由前面的bin_PROGRAMS
#指定的，如果前面是生成example,那么这里就是example_SOURCES，其它的类似标识也是一样

xxx_CPPFLAGS = -DCONFIG_DIR=\"$(sysconfdir)\" -DLIBRARY_DIR=\"$(pkglibdir)\"
#xxx_CPPFLAGS 这和Makefile文件中一样，表示C语言预处理参数，这里指定了DCONFIG_DIR，以后
#在程序中，就可以直接使用CONFIG_DIR。不要把这个和另一个CFLAGS混淆，后者表示编译器参数

xxx_LDFLAGS = -export-dynamic -lmemcached
#xxx_LDFLAGS 连接的时候所需库文件的标识，这个也就是对应一些如-l,-shared等选项

noinst_HEADERS = xxx.h
#这个表示该头文件只是参加可执行文件的编译，而不用安装到安装目录下。如果需要安装到系统中，
#可以用include_HEADERS来代替

INCLUDES = -I/usr/local/libmemcached/include/
#INCLUDES  链接时所需要的头文件

xxx_LDADD = $(top_builddir)/sx/libsession.a \
                $(top_builddir)/util/libutil.a
#xxx_LDADD 链接时所需要的库文件，这里表示需要两个库文件的支持

AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS= -I $(top_srcdir)/include/ -I $(top_srcdir)/src/mbedtls/include/
lib_LTLIBRARIES=libmbedtls.la
noinst_HEADERS=\
    include/aesni.h
libmbedtls_la_SOURCES=\
    aes.c\
 
#include <string/Base64.h>

int Base64Encode(const unsigned char *buffer,
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
    return bufferPtr->length;
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
    // 控制 base64 输出字符串不换行
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_push(b64, bio);
    *length = BIO_read(bio, *buffer, strlen(b64message));
    BIO_free_all(bio);
}