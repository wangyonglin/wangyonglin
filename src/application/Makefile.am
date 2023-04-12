ACLOCAL_AMFLAGS     = -I m4
AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS=\
    -I $(top_srcdir)/src/application/include/  \
    -I $(top_srcdir)/src/cJSON/include/   \
    -I $(top_srcdir)/src/SnowFlake/include/   \
    -I $(top_srcdir)/src/openssl/include/   \
    -I $(top_srcdir)/src/https/include/   \
    -I $(top_srcdir)/src/string/include/   \
    -I $(top_srcdir)/src/wangyonglin/include/

bin_PROGRAMS=wangyonglin
wangyonglin_SOURCES=\
    UserMain.c \
    AliyunHandler.c    \
    AliyunUtils.c \
    AliyunConfig.c \
    HTTPDServer.c \
    HTTPDHandler.c \
    HTTPDResult.c \
    WechatHttps.c \
    WechatPayment.c \
    AliyunHttps.c
wangyonglin_LDADD=\
    $(top_builddir)/src/SnowFlake/libSnowFlake.la \
    $(top_builddir)/src/cJSON/libcJSON.la \
    $(top_builddir)/src/openssl/libopenssl.la \
    $(top_builddir)/src/https/libhttps.la \
    $(top_builddir)/src/string/libstring.la \
    $(top_builddir)/src/wangyonglin/libwangyonglin.la \
    $(FLIBS) 
include_HEADERS =  $(top_srcdir)/src/application/include/*.h
#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl -lcurl 
