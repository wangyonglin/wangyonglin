AUTOMAKE_OPTIONS=foreign
lib_LTLIBRARIES=libwangyonglin.la
AM_CPPFLAGS=-I $(top_srcdir)/src/wangyonglin/include/
libwangyonglin_la_SOURCES=   \
    wangyonglin.c \
    pool.c  \
    daemonize.c    \
    lock.c   \
    log.c    \
    options.c  \
    conf.c  \
    message.c   \
    file.c \
    string.c \
    list.c \
    Unix_timestamp_converter.c
#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm  -lmysqlclient
