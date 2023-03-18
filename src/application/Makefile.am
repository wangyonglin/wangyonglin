ACLOCAL_AMFLAGS     = -I m4
AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS=\
    -I $(top_srcdir)/src/application/include/  \
    -I $(top_srcdir)/src/cJSON/include/   \
    -I $(top_srcdir)/src/SnowFlake/include/   \
    -I $(top_srcdir)/src/Encrypt/include/   \
    -I $(top_srcdir)/src/HTTPS/include/   \
    -I $(top_srcdir)/src/wangyonglin/include/

bin_PROGRAMS=wangyonglin
wangyonglin_SOURCES=\
    main.c \
    httpd_start.c \
    httpd_handler.c \
    aliapis.c    \
    aliutils.c \
    localapis.c \
    httpd_result.c

wangyonglin_LDADD=\
    $(top_builddir)/src/SnowFlake/libSnowFlake.la \
    $(top_builddir)/src/cJSON/libcJSON.la \
    $(top_builddir)/src/Encrypt/libEncrypt.la \
    $(top_builddir)/src/HTTPS/libHTTPS.la \
    $(top_builddir)/src/wangyonglin/libwangyonglin.la \
    $(FLIBS) 
include_HEADERS =  $(top_srcdir)/src/application/include/application/*.h
#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl -lcurl 
