AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm  -lssl -lcrypto -lpthread -levent_openssl 
AM_CPPFLAGS=    -I $(top_srcdir)/src/openssl/include/ \
                -I $(top_srcdir)/src/string/include/
lib_LTLIBRARIES=libopenssl.la
libopenssl_la_SOURCES=\
    HMAC_SHA1.c \
    base64.c    \
    URL.c   \
    aes_256_gcm.c


EXTRA_DIST =  $(top_srcdir)/src/openssl/include/*
