AUTOMAKE_OPTIONS=foreign
lib_LTLIBRARIES=libcomponent_mysqlclient.la
AM_CPPFLAGS=-I $(top_srcdir)/components/mysql/include/ -I $(top_srcdir)/include/
libcomponent_mysqlclient_la_SOURCES=   \
    mysql.c
#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm  -lmysqlclient
