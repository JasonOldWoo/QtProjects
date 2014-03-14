#-------------------------------------------------
#
# Project created by QtCreator 2013-09-05T15:22:33
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LanQQServer
TEMPLATE = app


SOURCES += main.cpp\
        lanqqserver.cpp \
    lancdb.cpp \
    tcpclientsocket.cpp \
    tcpserver.cpp \
    lcdbdataresol.cpp

HEADERS  += lanqqserver.h \
    lancdb.h \
    tcpclientsocket.h \
    tcpserver.h \
    DArrQueue.h \
    LCPdu.h \
    lcdbdataresol.h

FORMS    += lanqqserver.ui
