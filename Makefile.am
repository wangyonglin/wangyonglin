ACLOCAL_AMFLAGS     = -I m4
AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS= -I  $(top_srcdir)/include/
bin_PROGRAMS=wangyonglin
wangyonglin_SOURCES=application.c 
SUBDIRS = src/core src/cjson src/mqtt src/https src/string
wangyonglin_LDADD=\
    $(top_builddir)/src/core/libcore.la \
    $(top_builddir)/src/mqtt/libmqtt.la \
    $(top_builddir)/src/cjson/libcjson.la \
    $(top_builddir)/src/https/libhttps.la \
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
conf_DATA = $(top_srcdir)/conf/wangyonglin.conf

logsdir =   $(prefix)/logs
logs_DATA = $(top_srcdir)/logs/error.log

EXTRA_DIST  = README.md
EXTRA_DIST += $(top_srcdir)/include/*
EXTRA_DIST += $(top_srcdir)/conf/wangyonglin.conf
EXTRA_DIST += $(top_srcdir)/logs/error.log