SUBDIRS =\
    src/developer   \
    src/components/openssl    \
    src/components/snowflake    \
    src/components/mqtt \
    src/components/cjson \
    src/application


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