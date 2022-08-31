ACLOCAL_AMFLAGS     = -I m4
AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS= -I  $(top_srcdir)/include/
bin_PROGRAMS=tiger
tiger_SOURCES=application.c 
SUBDIRS = \
    src/unix   \
    src/cjson   \
    src/string  \
    src/toml    \
    src/mbedtls \
    src/paho_mqtt_c
tiger_LDADD=\
    $(top_builddir)/src/toml/libtoml.la \
    $(top_builddir)/src/mbedtls/libmbedtls.la \
    $(top_builddir)/src/paho_mqtt_c/libpaho_mqtt_c.la \
    $(top_builddir)/src/unix/libunix.la \
    $(top_builddir)/src/cjson/libcjson.la \
    $(top_builddir)/src/string/libstring.la \
    $(FLIBS) 

#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl -lcurl -lpaho-mqtt3c -lpaho-mqtt3a
#安装的各个目录和文件
includedir= $(prefix)/include
bindir=     $(prefix)/bin
libdir=     $(prefix)/lib
datadir=    $(prefix)/share
sysconfdir= $(prefix)/etc

#if ENABLE_EXAMPLES
#SUBDIRS            += examples
#endif
# Headers to install
#include_HEADERS= $(top_srcdir)/include/wangyonglin/*.h
#includedir=$(prefix)/include/wangyonglin




confdir =   $(prefix)/conf
conf_DATA = $(top_srcdir)/conf/tiger.conf

#logsdir =   $(prefix)/logs
#logs_DATA = $(top_srcdir)/logs/error.log

EXTRA_DIST  = README.md
EXTRA_DIST += $(top_srcdir)/include/*
EXTRA_DIST += $(top_srcdir)/conf/tiger.conf
#EXTRA_DIST += $(top_srcdir)/logs/error.log