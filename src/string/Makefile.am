AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm  -lssl -lcrypto -lpthread -levent_openssl 

AM_CPPFLAGS= \
    -I $(top_srcdir)/src/string/include/ \
    -I $(top_srcdir)/src/SnowFlake/include/ \
    -I $(top_srcdir)/src/zlog/include/

lib_LTLIBRARIES=libstring.la
libstring_la_SOURCES=\
    Unix_timestamp_converter.c \
    Stringex.c \
    StringexBase64.c \
    StringexSha256WithRsa.c \
    MemoryInject.c \
    ArrayList.c \
    StringexFile.c  \
    Sha256Rsa2048.c

EXTRA_DIST =  $(top_srcdir)/src/string/include/*