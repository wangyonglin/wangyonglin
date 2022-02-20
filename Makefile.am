AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS= -I  $(top_srcdir)/include/
bin_PROGRAMS=wangyonglin_test
wangyonglin_test_SOURCES=test.c 
SUBDIRS = src/core
wangyonglin_test_LDADD= $(top_builddir)/src/core/libcore.la \
    $(FLIBS)
#需要引用的动态库文件
LIBS += -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl -lcurl
#安装的各个目录和文件
bindir=     $(prefix)/bin
libdir=     $(prefix)/lib
datadir=    $(prefix)/share
sysconfdir= $(prefix)/etc

# Headers to install
#include_HEADERS= $(top_srcdir)/include/wangyonglin/*.h
#includedir=$(prefix)/include/wangyonglin

pkginclude_HEADERS=$(top_srcdir)/include/wangyonglin/*.h
pkgincludedir= $(prefix)/include/wangyonglin

confdir =   $(prefix)/conf
conf_DATA = $(top_srcdir)/conf/wangyonglin.conf

logsdir =   $(prefix)/logs