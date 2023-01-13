AUTOMAKE_OPTIONS=foreign
lib_LTLIBRARIES=libdeveloper.la
AM_CPPFLAGS=-I $(top_srcdir)/src/developer/include/
libdeveloper_la_SOURCES=   \
    config.c \
    allocate.c  \
    daemon.c    \
    pidlock.c   \
    log.c    \
    args.c  \
    conf.c  \
    message.c   \
    file.c \
    string.c \
    listitem.c \
    Unix_timestamp_converter.c
#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm  -lmysqlclient
