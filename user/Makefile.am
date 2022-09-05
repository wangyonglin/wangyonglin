ACLOCAL_AMFLAGS     = -I m4
AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS= -I  $(top_srcdir)/include/ -I $(top_srcdir)/user/include/
bin_PROGRAMS=tiger
tiger_SOURCES=main.c
tiger_LDADD=\
    $(top_builddir)/src/toml/libtoml.la \
    $(top_builddir)/src/mbedtls/libmbedtls.la \
    $(top_builddir)/src/paho_mqtt_c/libpaho_mqtt_c.la \
    $(top_builddir)/src/unix/libunix.la \
    $(top_builddir)/src/cjson/libcjson.la \
    $(top_builddir)/src/wangyonglin/libwangyonglin.la \
    $(FLIBS) 

#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl -lcurl -lpaho-mqtt3c -lpaho-mqtt3a
