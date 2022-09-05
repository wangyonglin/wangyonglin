AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS=-lpthread
AM_CPPFLAGS= -I $(top_srcdir)/include/ -I $(top_srcdir)/src/paho_mqtt_c/include/
lib_LTLIBRARIES=libpaho_mqtt_c.la
noinst_HEADERS=\
    include/Base64.h \
    include/Clients.h \
    include/Heap.h \
    include/LinkedList.h \
    include/Log.h \
    include/Messages.h \
 #   include/MQTTAsync.h \
 #   include/MQTTAsyncUtils.h \
    include/MQTTClient.h \
    include/MQTTClientPersistence.h \
    include/MQTTExportDeclarations.h \
    include/MQTTPacket.h \
    include/MQTTPacketOut.h \
    include/MQTTPersistenceDefault.h \
    include/MQTTPersistence.h \
    include/MQTTProperties.h \
    include/MQTTProtocolClient.h \
    include/MQTTProtocol.h \
    include/MQTTProtocolOut.h \
    include/MQTTReasonCodes.h \
    include/MQTTSubscribeOpts.h \
    include/MQTTTime.h \
    include/mutex_type.h \
    include/OsWrapper.h \
    include/Proxy.h \
    include/SHA1.h \
    include/SocketBuffer.h \
    include/Socket.h \
    include/SSLSocket.h \
    include/StackTrace.h \
    include/Thread.h \
    include/Tree.h \
    include/utf-8.h \
    include/WebSocket.h \
    include/VersionInfo.h
libpaho_mqtt_c_la_SOURCES=\
    Tree.c \
    Base64.c \
    Clients.c \
    Heap.c \
    LinkedList.c \
    Log.c \
    Messages.c \
   # MQTTAsync.c \
   # MQTTAsyncUtils.c \
    MQTTClient.c \
    MQTTPacket.c \
    MQTTPacketOut.c \
    MQTTPersistence.c \
    MQTTPersistenceDefault.c \
    MQTTProperties.c \
    MQTTProtocolClient.c \
    MQTTProtocolOut.c \
    MQTTReasonCodes.c \
    MQTTTime.c \
    MQTTVersion.c \
    OsWrapper.c \
    Proxy.c \
    SHA1.c \
    SocketBuffer.c \
    Socket.c \
    SSLSocket.c \
    StackTrace.c \
    Thread.c \
    utf-8.c \
    WebSocket.c