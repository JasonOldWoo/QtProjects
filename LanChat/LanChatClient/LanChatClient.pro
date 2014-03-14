#-------------------------------------------------
#
# Project created by QtCreator 2013-09-05T20:21:48
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LanChatClient
TEMPLATE = app


SOURCES += main.cpp\
        lanchatclient.cpp \
    signup.cpp \
    userinfo.cpp

HEADERS  += lanchatclient.h \
    signup.h \
    userinfo.h \
    LCPdu.h

FORMS    += lanchatclient.ui \
    signup.ui
