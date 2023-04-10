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

 aclocal && autoconf && autoheader && automake --add-missing
  ../configure --prefix=/usr/local/wangyonglin/
  /usr/bin/time -f "Total number of CPU-seconds consumed directly from each of the CPU cores: %U\nElapsed real wall clock time used by the process: %E" ./jwtcrack eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzUxMiJ9.eyJyb2xlIjoiYWRtaW4ifQ.RnWtv7Rjggm8LdMU3yLnz4ejgGAkIxoZwsCMuJlHMwTh7CJODDZWR8sVuNvo2ws25cbH9HWcp2n5WxpIZ9_v0g adimnps 9 sha512

32193C2A390F220B183E2B303A37193E

     char sec_key[] = "asdfgh";
    char data[] = "jkluiop";
    char encMessage[256] = {0};
    int encMessageLength = sizeof(encMessage);
    char *base64Text;
    hmac_sha1(sec_key, strlen(sec_key), data, strlen(data), encMessage, &encMessageLength);
    char *out = base64encode(encMessage, encMessageLength);
    printf("\t%s\r\n", out);
    printf("len: %d\n", strlen(out));
    free(out);
    设置时间
    timedatectl list-timezones |grep Shanghai
    timedatectl set-timezone Asia/Shanghai
typedef struct __gprmc__
{
UINT time;/* gps定位时间 */
char pos_state;/*gps状态位*/
float latitude;/*纬度 */
float longitude;/* 经度 */
float speed; /* 速度 */
float direction;/*航向 */
UINT date; /*日期 */
float declination; /* 磁偏角 */
char dd;
char mode;/* GPS模式位 */
}GPRMC;

log_errors  = On
lockfile = "/var/run/wangyonglin.pid"
logdir = "/home/wangyonglin/github/wangyonglin/logs"
[HTTPD]
address = "0.0.0.0"
port = 8080
timeout_in_secs = 15
message_text_max = 1024
[ALIIOT]
AccessKeyId =   ""
AccessKeySecret =   "&"
RegionId =  "cn-shanghai"
ProductKey = ""
DeviceName = ""
DeviceSecret = ""
Format  =   "JSON"
Version = "2018-01-20"
SignatureMethod = "HMAC-SHA1"
SignatureVersion = "1.0"
TopicFullName = "/user/get"
[WECHAT_PAYMENT]
appid   =   ""
secret = ""
mchid = ""
serial_no = ""
apiclient_key = "/home/wangyonglin/github/wangyonglin/cert/apiclient_key.pem"
notify_url = "https://api.wangyonglin.com/wechat/notify_url"