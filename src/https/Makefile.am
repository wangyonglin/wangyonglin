AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm -levent_openssl -lcrypto -levent -lssl -lcurl
AM_CPPFLAGS=    -I $(top_srcdir)/src/https/include/        \
                -I $(top_srcdir)/src/string/include/   \
                -I $(top_srcdir)/src/cJSON/include/
lib_LTLIBRARIES=libhttps.la
libhttps_la_SOURCES=\
        https_result.c \
        https_post.c \
        https_get.c \
        HTTPSUtils.c
include_HEADERS =  $(top_srcdir)/src/https/include/*.h