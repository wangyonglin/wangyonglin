AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm
AM_CPPFLAGS= -I $(top_srcdir)/src/SnowFlake/include/
lib_LTLIBRARIES=libSnowFlake.la
libSnowFlake_la_SOURCES=\
        IdGenerator.c  \
        IdGenOptions.c  \
        SnowFlake.c  \
        SnowWorkerM1.c  \
        SnowWorkerM2.c
EXTRA_DIST =  $(top_srcdir)/src/SnowFlake/include/*