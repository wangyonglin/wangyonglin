SUBDIRS =\
    src/wangyonglin   \
    src/openssl    \
    src/SnowFlake    \
    src/cJSON \
    src/https \
    src/string \
    src/application


#安装的各个目录和文件
#includedir= $(prefix)/include
bindir=     $(prefix)/bin
libdir=     $(prefix)/lib
datadir=    $(prefix)/share
sysconfdir= $(prefix)/etc
logdir =    $(prefix)/logs
confdir =   $(prefix)/conf
conf_DATA = $(top_srcdir)/conf/wangyonglin.conf

certdir = $(prefix)/cert
cert_DATA = $(top_srcdir)/cert/apiclient_cert.pem   \
            $(top_srcdir)/cert/apiclient_key.pem    \
            $(top_srcdir)/cert/apiclient_cert.p12

EXTRA_DIST  = README.md
EXTRA_DIST += $(top_srcdir)/conf/wangyonglin.conf
EXTRA_DIST += $(top_srcdir)/logs/error.log

EXTRA_DIST += $(top_srcdir)/cert/apiclient_cert.pem
EXTRA_DIST += $(top_srcdir)/cert/apiclient_key.pem
EXTRA_DIST += $(top_srcdir)/cert/apiclient_cert.p12
LIBS += -g -O2 -Wall -pedantic 