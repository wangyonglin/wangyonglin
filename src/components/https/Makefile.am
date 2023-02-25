AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm -levent_openssl -lcrypto -levent -lssl
AM_CPPFLAGS= -I $(top_srcdir)/src/components/https/include/
lib_LTLIBRARIES=libcomponent_https.la
libcomponent_https_la_SOURCES=\
        openssl_hostname_validation.c  \
        client.c  \
        hostcheck.c
