AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm
AM_CPPFLAGS= -I $(top_srcdir)/src/components/snowflake/include/
lib_LTLIBRARIES=libcomponent_snowflake.la
libcomponent_snowflake_la_SOURCES=\
        IdGenerator.c  \
        IdGenOptions.c  \
        SnowFlake.c  \
        SnowWorkerM1.c  \
        SnowWorkerM2.c
include_HEADERS =  $(top_srcdir)/src/components/snowflake/include/*.h