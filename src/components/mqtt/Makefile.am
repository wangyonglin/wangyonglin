AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm
AM_CPPFLAGS= -I $(top_srcdir)/src/components/mqtt/include/
lib_LTLIBRARIES=libcomponent_mqtt.la
#
#noinst_HEADERS=\
#    include/Base64.h \
#    include/Clients.h \
#    include/Heap.h \
#    include/LinkedList.h \
#    include/Log.h \
#    include/Messages.h \
#    include/MQTTAsync.h \
#    include/MQTTAsyncUtils.h \
#    include/MQTTClient.h \
#    include/MQTTClientPersistence.h \
#    include/MQTTExportDeclarations.h \
#    include/MQTTPacket.h \
#    include/MQTTPacketOut.h \
#    include/MQTTPersistenceDefault.h \
#    include/MQTTPersistence.h \
#    include/MQTTProperties.h \
#    include/MQTTProtocolClient.h \
#    include/MQTTProtocol.h \
#    include/MQTTProtocolOut.h \
#    include/MQTTReasonCodes.h \
#    include/MQTTSubscribeOpts.h \
#    include/MQTTTime.h \
#    include/mutex_type.h \
#    include/OsWrapper.h \
#    include/Proxy.h \
#    include/SHA1.h \
#    include/SocketBuffer.h \
#    include/Socket.h \
#    include/SSLSocket.h \
#    include/StackTrace.h \
#    include/Thread.h \
#    include/Tree.h \
#    include/utf-8.h \
#    include/WebSocket.h \
#    include/VersionInfo.h

libcomponent_mqtt_la_SOURCES=\
    Base64.c \
    LinkedList.c \
    Messages.c \
    MQTTPacket.c \
    MQTTProperties.c \
    MQTTTime.c \
    SHA1.c \
    StackTrace.c \
    WebSocket.c \
    Clients.c \
    Log.c \
    MQTTAsync.c \
    MQTTPacketOut.c \
    MQTTProtocolClient.c \
    MQTTVersion.c \
    SocketBuffer.c \
    Thread.c \
    Heap.c \
    MQTTAsyncUtils.c \
    MQTTPersistence.c \
    MQTTProtocolOut.c \
    OsWrapper.c \
    Socket.c \
    Tree.c \
    MQTTPersistenceDefault.c \
    MQTTReasonCodes.c \
    Proxy.c \
    SSLSocket.c \
    utf-8.c

include_HEADERS =  $(top_srcdir)/src/components/mqtt/include/*.h