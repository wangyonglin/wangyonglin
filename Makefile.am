SUBDIRS =\
    src/wangyonglin   \
    src/Encrypt    \
    src/SnowFlake    \
    src/cJSON \
    src/HTTPS \
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

EXTRA_DIST  = README.md
EXTRA_DIST += $(top_srcdir)/conf/tiger.conf
EXTRA_DIST += $(top_srcdir)/logs/error.log

 
LIBS += -g -O2 -Wall -pedantic 