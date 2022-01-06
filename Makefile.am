AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=skinny
skinny_SOURCES=main.c 
SUBDIRS = src
skinny_LDADD= \
    $(top_srcdir)/src/libskinny.la \
    $(FLIBS)
AM_CPPFLAGS=-I $(top_srcdir)/include/
LIBS += -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl
includedir= $(prefix)/include
bindir =    $(prefix)/bin
libdir =    $(prefix)/lib