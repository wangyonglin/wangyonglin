ACLOCAL_AMFLAGS     = -I m4
AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS=\
    -I $(top_srcdir)/src/zlog/include/  \
    -I $(top_srcdir)/src/application/include/  \
    -I $(top_srcdir)/src/cJSON/include/   \
    -I $(top_srcdir)/src/SnowFlake/include/   \
    -I $(top_srcdir)/src/openssl/include/   \
    -I $(top_srcdir)/src/https/include/   \
    -I $(top_srcdir)/src/string/include/   \
    -I $(top_srcdir)/src/wangyonglin/include/

sbin_PROGRAMS=wangyonglin
wangyonglin_SOURCES=\
    UserMain.c \
    AliyunHandler.c    \
    AliyunUtils.c \
    AliyunConfig.c \
    WebServer.c \
    HTTPDHandler.c \
    WechatHttps.c \
    WechatConfig.c \
    AliyunHttps.c \
    WechatHandler.c \
    ServerResponse.c \
    WechatConfigCertificates.c \
    WechatHandlerShipping.c \
    WechatConfigAuthorization.c \
    WechatHandlerNotifyUrl.c    \
    TiktokConfig.c              \
    TiktokSignature.c   \
    TiktokHandlerAccessToken.c   \
    TiktokHandlerTast.c
wangyonglin_LDADD=\
    $(top_builddir)/src/zlog/libzlog.la \
    $(top_builddir)/src/SnowFlake/libSnowFlake.la \
    $(top_builddir)/src/cJSON/libcJSON.la \
    $(top_builddir)/src/openssl/libopenssl.la \
    $(top_builddir)/src/https/libhttps.la \
    $(top_builddir)/src/string/libstring.la \
    $(top_builddir)/src/wangyonglin/libwangyonglin.la \
    $(FLIBS) 
EXTRA_DIST	 =  $(top_srcdir)/src/application/include/*
#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl -lcurl
