ACLOCAL_AMFLAGS     = -I m4
AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS=\
    -I $(top_srcdir)/include/  \
    -I $(top_srcdir)/src/components/mqtt/include/  \
    -I $(top_srcdir)/src/components/json/include/   \
    -I $(top_srcdir)/src/developer/include/

bin_PROGRAMS=tiger
tiger_SOURCES=\
    main.c
tiger_LDADD=\
    $(top_builddir)/src/components/mqtt/libcomponent_mqtt.la \
    $(top_builddir)/src/components/cjson/libcomponent_cjson.la \
    $(top_builddir)/src/developer/libdeveloper.la \
    $(FLIBS) 

#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl -lcurl 
