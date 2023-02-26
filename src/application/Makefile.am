ACLOCAL_AMFLAGS     = -I m4
AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS=\
    -I $(top_srcdir)/src/application/include/  \
    -I $(top_srcdir)/src/components/mqtt/include/  \
    -I $(top_srcdir)/src/components/cjson/include/   \
    -I $(top_srcdir)/src/components/snowflake/include/   \
    -I $(top_srcdir)/src/components/openssl/include/   \
    -I $(top_srcdir)/src/components/https/include/   \
    -I $(top_srcdir)/src/wangyonglin/include/

bin_PROGRAMS=wangyonglin
wangyonglin_SOURCES=\
    main.c \
    ali_signature.c \
    httpd_start.c \
    httpd_router.c \
    aliapis.c    \
    https_successful.c \
    aliutils.c
wangyonglin_LDADD=\
    $(top_builddir)/src/components/snowflake/libcomponent_snowflake.la \
    $(top_builddir)/src/components/mqtt/libcomponent_mqtt.la \
    $(top_builddir)/src/components/cjson/libcomponent_cjson.la \
    $(top_builddir)/src/components/openssl/libcomponent_openssl.la \
    $(top_builddir)/src/components/https/libcomponent_https.la \
    $(top_builddir)/src/wangyonglin/libwangyonglin.la \
    $(FLIBS) 
include_HEADERS =  $(top_srcdir)/src/application/include/application/*.h
#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl -lcurl 
