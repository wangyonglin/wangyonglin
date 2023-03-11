AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm -levent_openssl -lcrypto -levent -lssl
AM_CPPFLAGS= -I $(top_srcdir)/src/HTTPS/include/
lib_LTLIBRARIES=libHTTPS.la
libHTTPS_la_SOURCES=\
        openssl_hostname_validation.c  \
        client.c  \
        hostcheck.c
include_HEADERS =  $(top_srcdir)/src/HTTPS/include/https/*.h