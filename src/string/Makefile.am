AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm  -lssl -lcrypto -lpthread -levent_openssl 
AM_CPPFLAGS= \
    -I $(top_srcdir)/src/string/include/ \
    -I $(top_srcdir)/src/SnowFlake/include/

lib_LTLIBRARIES=libstring.la

libstring_la_SOURCES=\
    Unix_timestamp_converter.c \
    string_by_this.c \
    string_by_hex.c \
    string_by_timestamp.c \
    string_by_id.c \
    string_by_log.c \
    string_by_base64.c \
    string_by_inject.c \
    Stringex.c \
    StringexBase64.c \
    StringexSha256WithRsa.c
include_HEADERS =  $(top_srcdir)/src/string/include/*.h
