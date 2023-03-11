AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm  -lssl -lcrypto -lpthread -levent_openssl 
AM_CPPFLAGS= -I $(top_srcdir)/src/Encrypt/include/
lib_LTLIBRARIES=libEncrypt.la
libEncrypt_la_SOURCES=\
    SHA256WithRSA.c \
    HMAC_SHA1.c \
    Base64.c    \
    URL.c
include_HEADERS =  $(top_srcdir)/src/Encrypt/include/Encrypt/*.h
