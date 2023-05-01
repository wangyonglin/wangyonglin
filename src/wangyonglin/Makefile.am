AUTOMAKE_OPTIONS=foreign
lib_LTLIBRARIES=libwangyonglin.la
AM_CPPFLAGS=-I $(top_srcdir)/src/wangyonglin/include/ \
            -I $(top_srcdir)/src/string/include/    \
            -I $(top_srcdir)/src/zlog/include/ \
            -I $(top_srcdir)/src/SnowFlake/include/
libwangyonglin_la_SOURCES=   \
    MainConfig.c \
    pool.c  \
    MainDaemonize.c    \
    MainLock.c   \
    MainOption.c  \
    message.c   \
    MainFile.c
#需要引用的动态库文件
EXTRA_DIST =  $(top_srcdir)/src/wangyonglin/include/*
LIBS +=  -g -O0 -lm
