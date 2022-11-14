ACLOCAL_AMFLAGS     = -I m4
AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS=\
    -I $(top_srcdir)/include/  \
    -I $(top_srcdir)/components/mqtt/include/  \
    -I $(top_srcdir)/components/string/include/  \
    -I $(top_srcdir)/components/mysql/include/  \
    -I $(top_srcdir)/components/json/include/

bin_PROGRAMS=tiger
tiger_SOURCES=\
    SystemError.c \
    SystemDaemon.c    \
    SystemPidfile.c   \
    SystemLog.c   \
    SystemString.c \
    SystemFile.c \
    SystemConfig.c \
    SystemOptions.c \
    main.c  \
    HTTPSServer.c  \
    HTTPSHandler.c \
    MQTTManager.c   \
    MQTTAliyunSignature.c \
    SystemSignal.c \
    SystemTime.c 

tiger_LDADD=\
    $(top_builddir)/components/mqtt/libcomponent_mqtt.la \
    $(top_builddir)/components/string/libcomponent_string.la \
    $(top_builddir)/components/mysql/libcomponent_mysqlclient.la \
    $(top_builddir)/components/json/libcomponent_cjson.la \
    $(FLIBS) 

#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl -lcurl 
