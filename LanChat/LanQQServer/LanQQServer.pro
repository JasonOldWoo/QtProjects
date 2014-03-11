#-------------------------------------------------
#
# Project created by QtCreator 2013-09-05T15:22:33
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LanQQServer
TEMPLATE = app


SOURCES += main.cpp\
        lanqqserver.cpp \
    server.cpp \
    tcpclientsocket.cpp

HEADERS  += lanqqserver.h \
    server.h \
    tcpclientsocket.h

FORMS    += lanqqserver.ui
